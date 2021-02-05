--- source3/modules/vfs_zfsacl.c.orig	2021-02-05 12:50:57.335003000 -0500
+++ source3/modules/vfs_zfsacl.c	2021-02-05 12:52:42.951882000 -0500
@@ -63,6 +63,7 @@ static NTSTATUS zfs_get_nt_acl_common(struct connectio
 	int ret;
 	bool inherited_is_present = false;
 	bool is_dir;
+	uint16_t control = SEC_DESC_DACL_AUTO_INHERITED|SEC_DESC_SELF_RELATIVE;
 
 	if (VALID_STAT(smb_fname->st)) {
 		psbuf = &smb_fname->st;
@@ -149,11 +150,10 @@ static NTSTATUS zfs_get_nt_acl_common(struct connectio
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
