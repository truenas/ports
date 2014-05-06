index eec29f6..44d771e 100644
--- source3/smbd/aio.c 
+++ source3/smbd/aio.c 
@@ -688,6 +688,11 @@ NTSTATUS schedule_smb2_aio_read(connection_struct *conn,
 		return NT_STATUS_RETRY;
 	}
 
+	if (fsp->op == NULL) {
+		/* No AIO on internal opens. */
+		return NT_STATUS_RETRY;
+	}
+
 	if ((!min_aio_read_size || (smb_maxcnt < min_aio_read_size))
 	    && !SMB_VFS_AIO_FORCE(fsp)) {
 		/* Too small a read for aio request. */
@@ -839,6 +844,11 @@ NTSTATUS schedule_aio_smb2_write(connection_struct *conn,
 		return NT_STATUS_RETRY;
 	}
 
+	if (fsp->op == NULL) {
+		/* No AIO on internal opens. */
+		return NT_STATUS_RETRY;
+	}
+
 	if ((!min_aio_write_size || (in_data.length < min_aio_write_size))
 	    && !SMB_VFS_AIO_FORCE(fsp)) {
 		/* Too small a write for aio request. */
