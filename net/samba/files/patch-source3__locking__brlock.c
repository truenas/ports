--- source3/locking/brlock.c.orig	2021-07-07 08:38:32.218342000 -0400
+++ source3/locking/brlock.c	2021-07-07 08:38:39.511521000 -0400
@@ -1738,7 +1738,8 @@ static void byte_range_lock_flush(struct byte_range_lo
 	if (br_lck->num_locks == 0) {
 		/* No locks - delete this entry. */
 		NTSTATUS status = dbwrap_record_delete(br_lck->record);
-		if (!NT_STATUS_IS_OK(status)) {
+		if (!NT_STATUS_IS_OK(status) && 
+		    !NT_STATUS_EQUAL(status, NT_STATUS_NOT_FOUND)) {
 			DEBUG(0, ("delete_rec returned %s\n",
 				  nt_errstr(status)));
 			smb_panic("Could not delete byte range lock entry");
