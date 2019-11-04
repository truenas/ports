--- source3/modules/nfs4_acls.c.orig	2019-08-26 09:24:41.684793431 -0700
+++ source3/modules/nfs4_acls.c	2019-08-26 09:24:53.004762630 -0700
@@ -96,6 +96,8 @@ int smbacl4_get_vfs_params(struct connection_struct *c
 
 	params->map_full_control = lp_acl_map_full_control(SNUM(conn));
 
+	params->sort_aces = lp_parm_bool(SNUM(conn), SMBACL4_PARAM_TYPE_NAME, "sort_aces", False);
+
 	DEBUG(10, ("mode:%s, do_chown:%s, acedup: %s map full control:%s\n",
 		enum_smbacl4_modes[params->mode].name,
 		params->do_chown ? "true" : "false",
@@ -460,6 +462,7 @@ static NTSTATUS smb_get_nt_acl_nfs4_common(const SMB_S
 	size_t sd_size = 0;
 	struct security_ace *nt_ace_list = NULL;
 	struct security_acl *psa = NULL;
+	struct security_descriptor *psd = NULL;
 	TALLOC_CTX *frame = talloc_stackframe();
 	bool ok;
 
@@ -490,16 +493,24 @@ static NTSTATUS smb_get_nt_acl_nfs4_common(const SMB_S
 	}
 
 	DEBUG(10,("after make sec_acl\n"));
-	*ppdesc = make_sec_desc(
+
+	psd = make_sec_desc(
 		mem_ctx, SD_REVISION, smbacl4_get_controlflags(theacl),
 		(security_info & SECINFO_OWNER) ? &sid_owner : NULL,
 		(security_info & SECINFO_GROUP) ? &sid_group : NULL,
 		NULL, psa, &sd_size);
-	if (*ppdesc==NULL) {
-		DEBUG(2,("make_sec_desc failed\n"));
+
+	if (psd==NULL) {
+		DBG_ERR("make_sec_desc failed\n");
 		TALLOC_FREE(frame);
 		return NT_STATUS_NO_MEMORY;
 	}
+
+	if (psd->dacl && params->sort_aces) {
+		dacl_sort_into_canonical_order(psd->dacl->aces, (unsigned int)psd->dacl->num_aces);
+	}	
+
+	*ppdesc = psd;
 
 	DEBUG(10, ("smb_get_nt_acl_nfs4_common successfully exited with "
 		   "sd_size %d\n",
