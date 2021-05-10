--- src/rrdcached.c.orig	2017-06-06 20:13:54.693164693 +0200
+++ src/rrdcached.c	2019-01-25 22:45:29.468646783 +0100
@@ -41,6 +41,7 @@ static char *datadir;
 static char *daemon_address;
 static bool config_create_files = true;
 static bool config_collect_stats = true;
+static bool reported_connect_error = false;
 static rrdcreate_config_t rrdcreate_config = {.stepsize = 0,
                                               .heartbeat = 0,
                                               .rrarows = 1200,
@@ -284,10 +285,15 @@ static int rc_read(void) {
   rrd_clear_error();
   int status = rrdc_connect(daemon_address);
   if (status != 0) {
-    ERROR("rrdcached plugin: Failed to connect to RRDCacheD "
-          "at %s: %s (status=%d)",
-          daemon_address, rrd_get_error(), status);
+    if (!reported_connect_error) {
+      ERROR("rrdcached plugin: Failed to connect to RRDCacheD "
+            "at %s: %s (status=%d)",
+            daemon_address, rrd_get_error(), status);
+      reported_connect_error = true;
+    }
     return -1;
+  } else {
+    reported_connect_error = false;
   }
 
   rrdc_stats_t *head;
@@ -419,10 +425,15 @@ static int rc_write(const data_set_t *ds, const value_list_t *vl,
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
+      reported_connect_error = true;
+    }
     return -1;
+  } else {
+    reported_connect_error = false;
   }
 
   char *values_array[2] = {
@@ -445,8 +456,8 @@ static int rc_write(const data_set_t *ds, const value_list_t *vl,
       /* else: report the error and fail */
     }
 
-    ERROR("rrdcached plugin: rrdc_update (%s, [%s], 1) failed: %s (status=%i)",
-          filename, values_array[0], rrd_get_error(), status);
+    WARNING("rrdcached plugin: rrdc_update (%s, [%s], 1) failed: %s (status=%i)",
+            filename, values_array[0], rrd_get_error(), status);
     return -1;
   }
 
@@ -469,10 +480,15 @@ static int rc_flush(__attribute__((unused)) cdtime_t timeout, /* {{{ */
   rrd_clear_error();
   int status = rrdc_connect(daemon_address);
   if (status != 0) {
-    ERROR("rrdcached plugin: Failed to connect to RRDCacheD "
-          "at %s: %s (status=%d)",
-          daemon_address, rrd_get_error(), status);
+    if (!reported_connect_error) {
+      ERROR("rrdcached plugin: Failed to connect to RRDCacheD "
+            "at %s: %s (status=%d)",
+            daemon_address, rrd_get_error(), status);
+      reported_connect_error = true;
+    }
     return -1;
+  } else {
+    reported_connect_error = false;
   }
 
   bool retried = false;
