index 4ecf7ab..f062eba 100644
--- source3/modules/vfs_btrfs.c 
+++ source3/modules/vfs_btrfs.c 
@@ -98,6 +98,11 @@ static struct tevent_req *btrfs_copy_chunk_send(struct vfs_handle_struct *handle
 		return tevent_req_post(req, ev);
 	}
 
+	if (src_fsp->op == NULL || dest_fsp->op == NULL) {
+		tevent_req_nterror(req, NT_STATUS_INTERNAL_ERROR);
+		return tevent_req_post(req, ev);
+	}
+
 	init_strict_lock_struct(src_fsp,
 				src_fsp->op->global->open_persistent_id,
 				src_off,
