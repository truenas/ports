--- source3/modules/vfs_recycle.c.orig	2018-07-12 04:23:36.000000000 -0400
+++ source3/modules/vfs_recycle.c	2019-02-07 11:54:36.021712000 -0500
@@ -27,6 +27,7 @@
 #include "system/filesys.h"
 #include "../librpc/gen_ndr/ndr_netlogon.h"
 #include "auth.h"
+#include <sys/acl.h>
 
 #define ALLOC_CHECK(ptr, label) do { if ((ptr) == NULL) { DEBUG(0, ("recycle.bin: out of memory!\n")); errno = ENOMEM; goto label; } } while(0)
 
@@ -38,6 +39,47 @@ static int vfs_recycle_debug_level = DBG
 static int recycle_unlink(vfs_handle_struct *handle,
 			  const struct smb_filename *smb_fname);
 
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
@@ -267,6 +309,7 @@ static bool recycle_create_dir(vfs_handl
 	char *tok_str;
 	bool ret = False;
 	char *saveptr;
+	bool recycle_acl_is_set = False;
 
 	mode = recycle_directory_mode(handle);
 
@@ -313,11 +356,18 @@ static bool recycle_create_dir(vfs_handl
 				ret = False;
 				goto done;
 			}
+			if (!recycle_acl_is_set) {
+				if ( !lp_parm_bool(SNUM(handle->conn), "recycle", "preserveacl", False)
+				     && !set_recycle_acl(handle, smb_fname, mode) ) {
+					DBG_ERR("recycle: failed to prep ACL on %s\n", smb_fname->base_name);
+				} 
+			}
 			TALLOC_FREE(smb_fname);
 		}
 		if (strlcat(new_dir, "/", len+1) >= len+1) {
 			goto done;
 		}
+		recycle_acl_is_set = True;
 		mode = recycle_subdir_mode(handle);
 	}
 
