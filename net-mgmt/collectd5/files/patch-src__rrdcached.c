--- src/rrdcached.c.orig	2017-06-06 20:13:54.693164693 +0200
+++ src/rrdcached.c	2019-01-25 22:45:29.468646783 +0100
@@ -41,6 +41,7 @@ static char *datadir;
 static char *daemon_address;
 static _Bool config_create_files = 1;
 static _Bool config_collect_stats = 1;
+static _Bool reported_connect_error = 0;
 static rrdcreate_config_t rrdcreate_config = {
     /* stepsize = */ 0,
     /* heartbeat = */ 0,
@@ -269,11 +270,15 @@ static int try_reconnect(void) {
   rrd_clear_error();
   status = rrdc_connect(daemon_address);
   if (status != 0) {
-    ERROR("rrdcached plugin: Failed to reconnect to RRDCacheD "
-          "at %s: %s (status=%d)",
-          daemon_address, rrd_get_error(), status);
+    if (!reported_connect_error) {
+      ERROR("rrdcached plugin: Failed to reconnect to RRDCacheD "
+            "at %s: %s (status=%d)",
+            daemon_address, rrd_get_error(), status);
+      reported_connect_error = 1;
+    }
     return (-1);
   }
+  reported_connect_error = 0;
 
   INFO("rrdcached plugin: Successfully reconnected to RRDCacheD "
        "at %s",
@@ -304,11 +309,15 @@ static int rc_read(void) {
   rrd_clear_error();
   status = rrdc_connect(daemon_address);
   if (status != 0) {
-    ERROR("rrdcached plugin: Failed to connect to RRDCacheD "
-          "at %s: %s (status=%d)",
-          daemon_address, rrd_get_error(), status);
+    if (!reported_connect_error) {
+      ERROR("rrdcached plugin: Failed to connect to RRDCacheD "
+            "at %s: %s (status=%d)",
+            daemon_address, rrd_get_error(), status);
+      reported_connect_error = 1;
+    }
     return (-1);
   }
+  reported_connect_error = 0;
 
   while (42) {
     /* The RRD client lib does not provide any means for checking a
@@ -440,11 +449,15 @@ static int rc_write(const data_set_t *ds, const value_list_t *vl,
   rrd_clear_error();
   status = rrdc_connect(daemon_address);
   if (status != 0) {
-    ERROR("rrdcached plugin: Failed to connect to RRDCacheD "
-          "at %s: %s (status=%d)",
-          daemon_address, rrd_get_error(), status);
+    if (!reported_connect_error) {
+      ERROR("rrdcached plugin: Failed to connect to RRDCacheD "
+            "at %s: %s (status=%d)",
+            daemon_address, rrd_get_error(), status);
+      reported_connect_error = 1;
+    }
     return (-1);
   }
+  reported_connect_error = 0;
 
   while (42) {
     /* The RRD client lib does not provide any means for checking a
@@ -463,8 +463,8 @@
       /* else: report the error and fail */
     }
 
-    ERROR("rrdcached plugin: rrdc_update (%s, [%s], 1) failed: %s (status=%i)",
-          filename, values_array[0], rrd_get_error(), status);
+    WARNING("rrdcached plugin: rrdc_update (%s, [%s], 1) failed: %s (status=%i)",
+            filename, values_array[0], rrd_get_error(), status);
     return (-1);
   }
 
@@ -487,11 +500,15 @@ static int rc_flush(__attribute__((unused)) cdtime_t timeout, /* {{{ */
   rrd_clear_error();
   status = rrdc_connect(daemon_address);
   if (status != 0) {
-    ERROR("rrdcached plugin: Failed to connect to RRDCacheD "
-          "at %s: %s (status=%d)",
-          daemon_address, rrd_get_error(), status);
+    if (!reported_connect_error) {
+      ERROR("rrdcached plugin: Failed to connect to RRDCacheD "
+            "at %s: %s (status=%d)",
+            daemon_address, rrd_get_error(), status);
+      reported_connect_error = 1;
+    }
     return (-1);
   }
+  reported_connect_error = 0;
 
   while (42) {
     /* The RRD client lib does not provide any means for checking a
