index 82d059c..f6200ed 100644
--- source3/modules/vfs_default.c 
+++ source3/modules/vfs_default.c 
@@ -1393,6 +1393,10 @@ static struct tevent_req *vfswrap_copy_chunk_send(struct vfs_handle_struct *hand
 		off_t this_num = MIN(sizeof(vfs_cc_state->buf),
 				     num - vfs_cc_state->copied);
 
+		if (src_fsp->op == NULL) {
+			tevent_req_nterror(req, NT_STATUS_INTERNAL_ERROR);
+			return tevent_req_post(req, ev);
+		}
 		init_strict_lock_struct(src_fsp,
 					src_fsp->op->global->open_persistent_id,
 					src_off,
@@ -1426,6 +1430,11 @@ static struct tevent_req *vfswrap_copy_chunk_send(struct vfs_handle_struct *hand
 
 		src_off += ret;
 
+		if (dest_fsp->op == NULL) {
+			tevent_req_nterror(req, NT_STATUS_INTERNAL_ERROR);
+			return tevent_req_post(req, ev);
+		}
+
 		init_strict_lock_struct(dest_fsp,
 					dest_fsp->op->global->open_persistent_id,
 					dest_off,
