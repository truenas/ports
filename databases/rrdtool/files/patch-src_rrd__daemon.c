--- src/rrd_daemon.c.orig
+++ src/rrd_daemon.c
@@ -1659,6 +1659,7 @@
     rrd_file = rrd_open(file, &rrd, RRD_READONLY | RRD_LOCK);
     if (!rrd_file)
     {
+      unlink(file);
       rrd_free(&rrd);
       free (ci);
       RRDD_LOG (LOG_ERR, "handle_request_update: Could not read RRD file.");
