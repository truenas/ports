--- source3/modules/vfs_recycle.c.orig	2020-01-21 09:10:42.000000000 -0500
+++ source3/modules/vfs_recycle.c	2020-01-31 11:28:35.704064000 -0500
@@ -27,6 +27,7 @@
 #include "system/filesys.h"
 #include "../librpc/gen_ndr/ndr_netlogon.h"
 #include "auth.h"
+#include <sys/acl.h>
 
 #define ALLOC_CHECK(ptr, label) do { if ((ptr) == NULL) { DEBUG(0, ("recycle.bin: out of memory!\n")); errno = ENOMEM; goto label; } } while(0)
 
@@ -35,6 +36,47 @@ static int vfs_recycle_debug_level = DBG
 #undef DBGC_CLASS
 #define DBGC_CLASS vfs_recycle_debug_level
  
+static bool set_recycle_acl(vfs_handle_struct *handle, const struct smb_filename *smb_fname, mode_t mode)
+{
+	/*
+	 *	Redmine #73891
+	 *	When the ZFS aclmode property is set to restricted on a dataset and
+	 *	extended ACL entries exist on a file, then chmod() is denied. This breaks 
+	 *	user expectations because samba fails to set the posix mode of the recycle
+	 *	repository. This means that we need to strip the extended ACL first before
+	 *	chmod(). This behavior is non-portable.
+	 */
+	acl_t oldacl, newacl;
+	oldacl = acl_get_file(smb_fname->base_name, ACL_TYPE_NFS4);
+	if (oldacl == NULL) {
+		DBG_ERR("recycle: acl_get_file() failed for %s: %s\n", 
+			smb_fname->base_name, strerror(errno));
+		return false; 
+	}
+	newacl = acl_strip_np(oldacl, true);
+	if (newacl == NULL) {
+		DBG_ERR("recycle: acl_strip_np() failed for %s: %s\n", 
+			smb_fname->base_name, strerror(errno));
+		acl_free(oldacl);
+		return false; 
+	}
+	if ( acl_set_file(smb_fname->base_name, ACL_TYPE_NFS4, newacl) < 0 ) {
+		DBG_ERR("recycle: acl_set_file() failed for %s: %s\n",
+			smb_fname->base_name, strerror(errno));
+		acl_free(oldacl);
+		acl_free(newacl);
+		return false;
+	}
+	acl_free(oldacl);
+	acl_free(newacl);
+	if ( chmod(smb_fname->base_name, mode) < 0 ) {
+		DBG_ERR("recycle: failed to chmod %s to %o: %s\n", 
+			smb_fname->base_name, mode, strerror(errno));
+		return false; 
+	}
+	return true;
+}
+
 static const char *recycle_repository(vfs_handle_struct *handle)
 {
 	const char *tmp_str = NULL;
@@ -254,7 +296,7 @@ static off_t recycle_get_file_size(vfs_h
  * @param dname Directory tree to be created
  * @return Returns True for success
  **/
-static bool recycle_create_dir(vfs_handle_struct *handle, const char *dname)
+static bool recycle_create_dir(vfs_handle_struct *handle, const char *dname, bool is_repo)
 {
 	size_t len;
 	mode_t mode;
@@ -282,7 +324,7 @@ static bool recycle_create_dir(vfs_handl
 		}
 	}
 
-	/* Create directory tree if necessary */
+	/* Create directory tree if neccessary */
 	for(token = strtok_r(tok_str, "/", &saveptr); token;
 	    token = strtok_r(NULL, "/", &saveptr)) {
 		if (strlcat(new_dir, token, len+1) >= len+1) {
@@ -292,7 +334,6 @@ static bool recycle_create_dir(vfs_handl
 			DEBUG(10, ("recycle: dir %s already exists\n", new_dir));
 		else {
 			struct smb_filename *smb_fname = NULL;
-			int retval;
 
 			DEBUG(5, ("recycle: creating new dir %s\n", new_dir));
 
@@ -305,19 +346,15 @@ static bool recycle_create_dir(vfs_handl
 				goto done;
 			}
 
-			retval = SMB_VFS_NEXT_MKDIRAT(handle,
-					handle->conn->cwd_fsp,
-					smb_fname,
-					mode);
-			if (retval != 0) {
-				DBG_WARNING("recycle: mkdirat failed "
-					"for %s with error: %s\n",
-					new_dir,
-					strerror(errno));
+			if (SMB_VFS_NEXT_MKDIRAT(handle, handle->conn->cwd_fsp, smb_fname, mode) != 0) {
+				DEBUG(1,("recycle: mkdir failed for %s with error: %s\n", new_dir, strerror(errno)));
 				TALLOC_FREE(smb_fname);
 				ret = False;
 				goto done;
 			}
+			if (is_repo && !set_recycle_acl(handle, smb_fname, recycle_directory_mode(handle))) {
+				DBG_ERR("recycle: failed to prep ACL on %s\n", smb_fname->base_name);
+			}
 			TALLOC_FREE(smb_fname);
 		}
 		if (strlcat(new_dir, "/", len+1) >= len+1) {
@@ -609,7 +646,7 @@ static int recycle_unlink_internal(vfs_h
 		DEBUG(10, ("recycle: Directory already exists\n"));
 	} else {
 		DEBUG(10, ("recycle: Creating directory %s\n", temp_name));
-		if (recycle_create_dir(handle, temp_name) == False) {
+		if (recycle_create_dir(handle, temp_name, false) == False) {
 			DEBUG(3, ("recycle: Could not create directory, "
 				  "purging %s...\n",
 				  smb_fname_str_dbg(smb_fname)));
@@ -709,6 +746,50 @@ done:
 	return rc;
 }
 
+static int recycle_connect(struct vfs_handle_struct *handle,
+                            const char *service, const char *user)
+{
+	int ret = -1;
+	char *repository = NULL;
+	char *recycle_path = NULL;
+	const struct loadparm_substitution *lp_sub =
+		loadparm_s3_global_substitution();
+	repository = talloc_sub_full(NULL, lp_servicename(talloc_tos(), lp_sub, SNUM(handle->conn)),
+					handle->conn->session_info->unix_info->unix_name,
+					handle->conn->connectpath,
+					handle->conn->session_info->unix_token->gid,
+					handle->conn->session_info->unix_info->sanitized_username,
+					handle->conn->session_info->info->domain_name,
+					recycle_repository(handle));
+	ALLOC_CHECK(repository, done);
+	trim_char(repository, '\0', '/');
+
+	if(!repository || *(repository) == '\0') {
+		/*
+		 * In case repository is not set, return -1 on connect which will prevent users
+		 * from connecting to the share. Otherwise, deletion can result in unexpected
+		 * data loss. 
+		 */
+		DBG_ERR("recycle: repository path not set, files will be automatically deleted\n");
+		DBG_ERR("recycle: denying access to share\n");
+		errno = EINVAL;
+		goto done;
+	}
+	recycle_path = talloc_asprintf(talloc_tos(), "%s/%s", handle->conn->connectpath, repository);
+	if ( !lp_parm_bool(SNUM(handle->conn), "recycle", "preserveacl", False) &&
+	     !directory_exist(recycle_path) ) {
+		DBG_INFO("recycle_path is: [%s]\n", recycle_path);
+		if (!recycle_create_dir(handle, recycle_path, true)) {
+			DBG_ERR("Failed to create recycle path [%s]\n", recycle_path);
+		}
+	}
+	TALLOC_FREE(recycle_path);
+	ret = SMB_VFS_NEXT_CONNECT(handle, service, user);
+done:
+	TALLOC_FREE(repository);
+	return ret; 
+}
+
 static int recycle_unlinkat(vfs_handle_struct *handle,
 		struct files_struct *dirfsp,
 		const struct smb_filename *smb_fname,
@@ -732,7 +813,8 @@ static int recycle_unlinkat(vfs_handle_s
 }
 
 static struct vfs_fn_pointers vfs_recycle_fns = {
-	.unlinkat_fn = recycle_unlinkat
+	.unlinkat_fn = recycle_unlinkat,
+	.connect_fn = recycle_connect
 };
 
 static_decl_vfs;
