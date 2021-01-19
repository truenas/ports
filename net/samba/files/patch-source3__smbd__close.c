--- source3/smbd/close.c.orig	2020-07-02 07:12:21.000000000 -0400
+++ source3/smbd/close.c	2020-10-22 14:24:53.857059000 -0400
@@ -447,8 +447,12 @@ static NTSTATUS close_remove_share_mode(files_struct *
 	    && !is_ntfs_stream_smb_fname(fsp->fsp_name)) {
 
 		status = delete_all_streams(conn, fsp->fsp_name);
+		if (NT_STATUS_EQUAL(status, NT_STATUS_ACCESS_DENIED)) {
+			DEBUG(5, ("delete_all_streams failed with permission"
+				  "error. Continuing with delete\n"));
+		}
 
-		if (!NT_STATUS_IS_OK(status)) {
+		else if (!NT_STATUS_IS_OK(status)) {
 			DEBUG(5, ("delete_all_streams failed: %s\n",
 				  nt_errstr(status)));
 			goto done;
@@ -1204,7 +1208,12 @@ static NTSTATUS close_directory(struct smb_request *re
 		    && !is_ntfs_stream_smb_fname(fsp->fsp_name)) {
 
 			status = delete_all_streams(fsp->conn, fsp->fsp_name);
-			if (!NT_STATUS_IS_OK(status)) {
+
+			if (NT_STATUS_EQUAL(status, NT_STATUS_ACCESS_DENIED)) {
+				DEBUG(5, ("delete_all_streams failed with permission"
+					  "error. Continuing with delete\n"));
+			}
+			else if (!NT_STATUS_IS_OK(status)) {
 				DEBUG(5, ("delete_all_streams failed: %s\n",
 					  nt_errstr(status)));
 				file_free(req, fsp);
