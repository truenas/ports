--- source3/modules/vfs_zfsacl.c.orig	2021-01-14 03:29:57.000000000 -0500
+++ source3/modules/vfs_zfsacl.c	2021-06-08 02:50:46.193015000 -0400
@@ -41,6 +41,7 @@ struct zfsacl_config_data {
 	bool zfsacl_map_dacl_protected;
 	bool zfsacl_denymissingspecial;
 	bool zfsacl_block_special;
+	bool zfsacl_legacy_delete;
 };
 
 /* zfs_get_nt_acl()
@@ -63,6 +64,7 @@ static NTSTATUS zfs_get_nt_acl_common(struct connectio
 	int ret;
 	bool inherited_is_present = false;
 	bool is_dir;
+	uint16_t control = SEC_DESC_DACL_AUTO_INHERITED|SEC_DESC_SELF_RELATIVE;
 
 	if (VALID_STAT(smb_fname->st)) {
 		psbuf = &smb_fname->st;
@@ -114,7 +116,7 @@ static NTSTATUS zfs_get_nt_acl_common(struct connectio
 
 		if (is_dir &&
 		    (aceprop.aceMask & SMB_ACE4_ADD_FILE) &&
-		    (special != 0))
+		    ((special != 0) || config->zfsacl_legacy_delete))
 		{
 			aceprop.aceMask |= SMB_ACE4_DELETE_CHILD;
 		}
@@ -149,11 +151,10 @@ static NTSTATUS zfs_get_nt_acl_common(struct connectio
 	if (!inherited_is_present && config->zfsacl_map_dacl_protected) {
 		DBG_DEBUG("Setting SEC_DESC_DACL_PROTECTED on [%s]\n",
 			  smb_fname_str_dbg(smb_fname));
-		smbacl4_set_controlflags(pacl,
-					 SEC_DESC_DACL_PROTECTED |
-					 SEC_DESC_SELF_RELATIVE);
+		control |= SEC_DESC_DACL_PROTECTED;
 	}
 #endif
+	smbacl4_set_controlflags(pacl, control);
 	*ppacl = pacl;
 	return NT_STATUS_OK;
 }
@@ -591,6 +592,9 @@ static int zfsacl_connect(struct vfs_handle_struct *ha
 
 	config->zfsacl_block_special = lp_parm_bool(SNUM(handle->conn),
 				"zfsacl", "block_special", true);
+
+	config->zfsacl_legacy_delete = lp_parm_bool(SNUM(handle->conn),
+				"zfsacl", "enable_legacy_insecure_delete_mapping", false);
 
 	ret = smbacl4_get_vfs_params(handle->conn, &config->nfs4_params);
 	if (ret < 0) {
