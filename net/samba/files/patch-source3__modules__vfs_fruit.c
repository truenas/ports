--- source3/modules/vfs_fruit.c.orig	2021-01-14 03:29:57.000000000 -0500
+++ source3/modules/vfs_fruit.c	2021-06-02 04:46:25.426396000 -0400
@@ -892,6 +892,7 @@ static bool readdir_attr_meta_finderi_stream(
 	if (stream_name == NULL) {
 		return false;
 	}
+	stream_name->vss_tval.tv_sec = smb_fname->vss_tval.tv_sec;
 
 	ret = SMB_VFS_STAT(handle->conn, stream_name);
 	if (ret != 0) {
@@ -1066,6 +1067,7 @@ static uint64_t readdir_attr_rfork_size_stream(
 	if (stream_name == NULL) {
 		return 0;
 	}
+	stream_name->vss_tval.tv_sec = smb_fname->vss_tval.tv_sec;
 
 	ret = SMB_VFS_STAT(handle->conn, stream_name);
 	if (ret != 0) {
@@ -2011,6 +2013,15 @@ static int fruit_unlink_internal(vfs_handle_struct *ha
 	}
 
 	/*
+	 * Skip resource fork deletion for SMB_POSIX_PATH_UNLINK
+	 * otherwise we will hit assert_valid_stream_smb_fname()
+	 */
+	if ((config->rsrc == FRUIT_RSRC_STREAM) &&
+	    (smb_fname->flags & SMB_FILENAME_POSIX_PATH)) {
+		goto out;
+	}
+
+	/*
 	 * A request to delete the base file. Because 0 byte resource
 	 * fork streams are not listed by fruit_streaminfo,
 	 * delete_all_streams() can't remove 0 byte resource fork
@@ -2024,6 +2035,7 @@ static int fruit_unlink_internal(vfs_handle_struct *ha
 	if (rsrc_smb_fname == NULL) {
 		return -1;
 	}
+	rsrc_smb_fname->vss_tval.tv_sec = smb_fname->vss_tval.tv_sec;
 
 	rc = fruit_unlink_rsrc(handle, dirfsp, rsrc_smb_fname, true);
 	if ((rc != 0) && (errno != ENOENT)) {
@@ -2034,6 +2046,7 @@ static int fruit_unlink_internal(vfs_handle_struct *ha
 	}
 	TALLOC_FREE(rsrc_smb_fname);
 
+out:
 	return SMB_VFS_NEXT_UNLINKAT(handle,
 			dirfsp,
 			smb_fname,
@@ -2134,6 +2147,7 @@ static int fruit_rmdir_internal(struct vfs_handle_stru
 			DBG_ERR("synthetic_smb_fname failed\n");
 			return -1;
 		}
+		ad_smb_fname->vss_tval.tv_sec = smb_fname->vss_tval.tv_sec;
 
 		/*
 		 * Check whether it's a valid AppleDouble file, if
@@ -2201,7 +2215,14 @@ static ssize_t fruit_pread_meta_stream(vfs_handle_stru
 	}
 
 	nread = SMB_VFS_NEXT_PREAD(handle, fsp, data, n, offset);
-	if (nread == -1 || nread == n) {
+	if (nread <= 0 || nread == n) {
+		/*
+		 * fruit_meta_open_stream() removes O_CREAT flag
+		 * from xattr open. This results in vfs_streams_xattr
+		 * not generating an FSP extension for the files_struct
+		 * and causes subsequent pread() of stream to return
+		 * nread=0 if pread() occurs before pwrite().
+		 */
 		return nread;
 	}
 
@@ -3446,6 +3467,7 @@ static NTSTATUS delete_invalid_meta_stream(
 	if (sname == NULL) {
 		return NT_STATUS_NO_MEMORY;
 	}
+	sname->vss_tval.tv_sec = smb_fname->vss_tval.tv_sec;
 
 	ret = SMB_VFS_NEXT_UNLINKAT(handle,
 			handle->conn->cwd_fsp,
@@ -4599,6 +4621,7 @@ static void fruit_offload_write_done(struct tevent_req
 		if (tevent_req_nomem(src_fname_tmp, req)) {
 			return;
 		}
+		src_fname_tmp->vss_tval.tv_sec = state->src_fsp->fsp_name->vss_tval.tv_sec;
 
 		if (is_ntfs_default_stream_smb_fname(src_fname_tmp)) {
 			TALLOC_FREE(src_fname_tmp);
@@ -4615,6 +4638,7 @@ static void fruit_offload_write_done(struct tevent_req
 			TALLOC_FREE(src_fname_tmp);
 			return;
 		}
+		dst_fname_tmp->vss_tval.tv_sec = state->dst_fsp->fsp_name->vss_tval.tv_sec;
 
 		status = copy_file(req,
 				   state->handle->conn,
