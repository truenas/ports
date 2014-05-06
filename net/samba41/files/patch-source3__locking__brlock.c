index 78c2538..df689a7 100644
--- source3/locking/brlock.c 
+++ source3/locking/brlock.c 
@@ -1526,12 +1526,18 @@ void brl_close_fnum(struct messaging_context *msg_ctx,
 bool brl_mark_disconnected(struct files_struct *fsp)
 {
 	uint32_t tid = fsp->conn->cnum;
-	uint64_t smblctx = fsp->op->global->open_persistent_id;
+	uint64_t smblctx;
 	uint64_t fnum = fsp->fnum;
 	unsigned int i;
 	struct server_id self = messaging_server_id(fsp->conn->sconn->msg_ctx);
 	struct byte_range_lock *br_lck = NULL;
 
+	if (fsp->op == NULL) {
+		return false;
+	}
+
+	smblctx = fsp->op->global->open_persistent_id;
+
 	if (!fsp->op->global->durable) {
 		return false;
 	}
@@ -1586,12 +1592,18 @@ bool brl_mark_disconnected(struct files_struct *fsp)
 bool brl_reconnect_disconnected(struct files_struct *fsp)
 {
 	uint32_t tid = fsp->conn->cnum;
-	uint64_t smblctx = fsp->op->global->open_persistent_id;
+	uint64_t smblctx;
 	uint64_t fnum = fsp->fnum;
 	unsigned int i;
 	struct server_id self = messaging_server_id(fsp->conn->sconn->msg_ctx);
 	struct byte_range_lock *br_lck = NULL;
 
+	if (fsp->op == NULL) {
+		return false;
+	}
+
+	smblctx = fsp->op->global->open_persistent_id;
+
 	if (!fsp->op->global->durable) {
 		return false;
 	}
