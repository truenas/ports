index d94ee11..d1e7df6 100644
--- source3/smbd/files.c 
+++ source3/smbd/files.c 
@@ -93,7 +93,7 @@ NTSTATUS file_new(struct smb_request *req, connection_struct *conn,
 
 	GetTimeOfDay(&fsp->open_time);
 
-	if (sconn->conn) {
+	if (req) {
 		struct smbXsrv_open *op = NULL;
 		NTTIME now = timeval_to_nttime(&fsp->open_time);
 
@@ -108,6 +108,9 @@ NTSTATUS file_new(struct smb_request *req, connection_struct *conn,
 		op->compat = fsp;
 		fsp->fnum = op->local_id;
 		fsp->fh->gen_id = smbXsrv_open_hash(op);
+	} else {
+		DEBUG(10, ("%s: req==NULL, INTERNAL_OPEN_ONLY, smbXsrv_open "
+			   "allocated\n", __func__));
 	}
 
 	/*
