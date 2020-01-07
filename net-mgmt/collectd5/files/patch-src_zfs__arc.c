--- src/zfs_arc.c
+++ src/zfs_arc.c
@@ -278,7 +281,7 @@ static int za_read(void) {
    * Stop trying if we ever fail to read it, so we don't spam the log.
    */
   static int l2_size_avail = 1;
-  if (l2_size_avail && za_read_gauge(ksp, "l2_size", "cache_size", "L2") != 0)
+  if (l2_size_avail && za_read_gauge(ksp, "l2_asize", "cache_size", "L2") != 0)
     l2_size_avail = 0;
 
   /* Operations */
