--- source3/smbd/close.c.orig	2020-11-03 15:12:33.842120000 -0500
+++ source3/smbd/close.c	2020-11-03 15:17:33.730105000 -0500
@@ -164,6 +164,15 @@ NTSTATUS delete_all_streams(connection_struct *conn,
 		TALLOC_FREE(frame);
 		return NT_STATUS_OK;
 	}
+	else if (NT_STATUS_EQUAL(status, NT_STATUS_ACCESS_DENIED)) {
+		DBG_DEBUG("Attempt to look up stream info failed with "
+			"ACCESS_DENIED. Temporarily elevating privileges "
+			"in order to determine whether streams exist on file.");
+		become_root();
+		status = vfs_streaminfo(conn, NULL, smb_fname, talloc_tos(),
+					&num_streams, &stream_info);
+		unbecome_root();
+	}
 
 	if (!NT_STATUS_IS_OK(status)) {
 		DEBUG(10, ("vfs_streaminfo failed: %s\n",
