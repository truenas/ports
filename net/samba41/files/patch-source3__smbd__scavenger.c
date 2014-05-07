index e6e2878..122305e 100644
--- source3/smbd/scavenger.c 
+++ source3/smbd/scavenger.c 
@@ -418,6 +418,9 @@ void scavenger_schedule_disconnected(struct files_struct *fsp)
 	struct scavenger_message msg;
 	DATA_BLOB msg_blob;
 
+	if (fsp->op == NULL) {
+		return;
+	}
 	nttime_to_timeval(&disconnect_time, fsp->op->global->disconnect_time);
 	timeout_usec = 1000 * fsp->op->global->durable_timeout_msec;
 	until = timeval_add(&disconnect_time,
