--- src/zfs_arc.c.orig	2019-06-13 09:13:42 UTC
+++ src/zfs_arc.c
@@ -197,6 +197,9 @@ static void za_submit_ratio(const char *type_instance, gauge_t hits,
   za_submit_gauge("cache_ratio", type_instance, ratio);
 }
 
+static value_to_rate_state_t arc_hits_state, arc_misses_state, l2_hits_state,
+    l2_misses_state;
+
 static int za_read(void) {
   gauge_t arc_hits, arc_misses, l2_hits, l2_misses;
   kstat_t *ksp = NULL;
@@ -278,7 +281,7 @@ static int za_read(void) {
    * Stop trying if we ever fail to read it, so we don't spam the log.
    */
   static int l2_size_avail = 1;
-  if (l2_size_avail && za_read_gauge(ksp, "l2_size", "cache_size", "L2") != 0)
+  if (l2_size_avail && za_read_gauge(ksp, "l2_asize", "cache_size", "L2") != 0)
     l2_size_avail = 0;
 
   /* Operations */
