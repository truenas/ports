--- source3/smbd/close.c.orig	2020-10-08 20:46:24.346293000 -0400
+++ source3/smbd/close.c	2020-10-08 20:47:34.858038000 -0400
@@ -666,7 +666,7 @@ static void assert_no_pending_aio(struct files_struct 
 		 * fsp->aio_requests[x], causing a crash.
 		 */
 		while (fsp->num_aio_requests != 0) {
-			TALLOC_FREE(fsp->aio_requests[0]);
+			talloc_free(fsp->aio_requests[0]);
 		}
 		return;
 	}
