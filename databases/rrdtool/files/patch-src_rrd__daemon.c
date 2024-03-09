--- src/rrd_daemon.c.orig	2024-03-08 14:48:11.734284000 -0600
+++ src/rrd_daemon.c	2024-03-08 14:48:42.759007000 -0600
@@ -1677,6 +1677,7 @@ static int handle_request_update(
         rrd_init(&rrd);
         rrd_file = rrd_open(file, &rrd, RRD_READONLY | RRD_LOCK);
         if (!rrd_file) {
+            unlink(file);
             rrd_free(&rrd);
             free(ci->file);
             free(ci);
