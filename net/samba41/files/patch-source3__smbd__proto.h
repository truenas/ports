index 1e0d06d..6153a49 100644
--- source3/smbd/proto.h 
+++ source3/smbd/proto.h 
@@ -620,9 +620,6 @@ NTSTATUS change_dir_owner_to_parent(connection_struct *conn,
 				    SMB_STRUCT_STAT *psbuf);
 bool is_stat_open(uint32 access_mask);
 bool is_deferred_open_async(const void *ptr);
-NTSTATUS open_file_fchmod(connection_struct *conn,
-			  struct smb_filename *smb_fname,
-			  files_struct **result);
 NTSTATUS create_directory(connection_struct *conn, struct smb_request *req,
 			  struct smb_filename *smb_dname);
 void msg_file_was_renamed(struct messaging_context *msg,
