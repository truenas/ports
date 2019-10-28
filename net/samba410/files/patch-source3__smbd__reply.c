--- source3/smbd/reply.c.orig	2019-05-22 03:16:26.000000000 -0700
+++ source3/smbd/reply.c	2019-06-10 12:37:49.685400772 -0700
@@ -6674,10 +6675,13 @@ NTSTATUS rename_internals_fsp(connection
 		return status;
 	}
 
+#if 0
 	if (file_has_open_streams(fsp)) {
 		return NT_STATUS_ACCESS_DENIED;
 	}
 
+#endif
+
 	/* Make a copy of the dst smb_fname structs */
 
 	smb_fname_dst = cp_smb_filename(ctx, smb_fname_dst_in);
@@ -6824,11 +6828,25 @@ NTSTATUS rename_internals_fsp(connection
 	dst_exists = SMB_VFS_STAT(conn, smb_fname_dst) == 0;
 
 	if(!replace_if_exists && dst_exists) {
+		struct file_id fileid = vfs_file_id_from_sbuf(conn,
+		    &smb_fname_dst->st);
 		DEBUG(3, ("rename_internals_fsp: dest exists doing rename "
 			  "%s -> %s\n", smb_fname_str_dbg(fsp->fsp_name),
 			  smb_fname_str_dbg(smb_fname_dst)));
-		status = NT_STATUS_OBJECT_NAME_COLLISION;
-		goto out;
+		if ((fsp->file_id.devid != fileid.devid) || 
+		    (fsp->file_id.inode != fileid.inode)) { 
+			status = NT_STATUS_OBJECT_NAME_COLLISION;
+			goto out;
+		}
+		else {
+		/*
+		 *	Path is apparently case-insensitive. On FreeBSD,
+		 *	rename() to only change case is no-op, bail out early
+		 *	here.
+		 */
+			status = NT_STATUS_OK;
+			goto out;
+		}
 	}
 
 	if (dst_exists) {
