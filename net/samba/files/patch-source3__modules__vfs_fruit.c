--- source3/modules/vfs_fruit.c.orig	2020-04-07 02:45:51.000000000 -0400
+++ source3/modules/vfs_fruit.c	2020-12-07 08:52:21.256596000 -0500
@@ -895,6 +895,7 @@ static bool readdir_attr_meta_finderi_stream(
 	if (stream_name == NULL) {
 		return false;
 	}
+	stream_name->vss_tval.tv_sec = smb_fname->vss_tval.tv_sec;
 
 	ret = SMB_VFS_STAT(handle->conn, stream_name);
 	if (ret != 0) {
@@ -1069,6 +1070,7 @@ static uint64_t readdir_attr_rfork_size_stream(
 	if (stream_name == NULL) {
 		return 0;
 	}
+	stream_name->vss_tval.tv_sec = smb_fname->vss_tval.tv_sec;
 
 	ret = SMB_VFS_STAT(handle->conn, stream_name);
 	if (ret != 0) {
@@ -2021,6 +2023,7 @@ static int fruit_unlink_internal(vfs_handle_struct *ha
 	if (rsrc_smb_fname == NULL) {
 		return -1;
 	}
+	rsrc_smb_fname->vss_tval.tv_sec = smb_fname->vss_tval.tv_sec;
 
 	rc = fruit_unlink_rsrc(handle, dirfsp, rsrc_smb_fname, true);
 	if ((rc != 0) && (errno != ENOENT)) {
@@ -2131,6 +2134,7 @@ static int fruit_rmdir_internal(struct vfs_handle_stru
 			DBG_ERR("synthetic_smb_fname failed\n");
 			return -1;
 		}
+		ad_smb_fname->vss_tval.tv_sec = smb_fname->vss_tval.tv_sec;
 
 		/*
 		 * Check whether it's a valid AppleDouble file, if
@@ -2193,7 +2197,14 @@ static ssize_t fruit_pread_meta_stream(vfs_handle_stru
 	int ret;
 
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
 
@@ -3438,6 +3449,7 @@ static NTSTATUS delete_invalid_meta_stream(
 	if (sname == NULL) {
 		return NT_STATUS_NO_MEMORY;
 	}
+	sname->vss_tval.tv_sec = smb_fname->vss_tval.tv_sec;
 
 	ret = SMB_VFS_NEXT_UNLINKAT(handle,
 			handle->conn->cwd_fsp,
@@ -4597,6 +4609,7 @@ static void fruit_offload_write_done(struct tevent_req
 		if (tevent_req_nomem(src_fname_tmp, req)) {
 			return;
 		}
+		src_fname_tmp->vss_tval.tv_sec = state->src_fsp->fsp_name->vss_tval.tv_sec;
 
 		if (is_ntfs_default_stream_smb_fname(src_fname_tmp)) {
 			TALLOC_FREE(src_fname_tmp);
@@ -4613,6 +4626,7 @@ static void fruit_offload_write_done(struct tevent_req
 			TALLOC_FREE(src_fname_tmp);
 			return;
 		}
+		dst_fname_tmp->vss_tval.tv_sec = state->dst_fsp->fsp_name->vss_tval.tv_sec;
 
 		status = copy_file(req,
 				   state->handle->conn,
