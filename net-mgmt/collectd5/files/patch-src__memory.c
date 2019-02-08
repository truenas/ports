--- src/memory.c	2017-11-18 04:03:27.358750191 -0500
+++ src/memory.c	2017-12-22 11:11:29.803093000 -0500
@@ -228,14 +228,16 @@ static int memory_read_internal(value_list_t *vl) {
    * vm.stats.vm.v_wire_count: 37526
    * vm.stats.vm.v_active_count: 55239
    * vm.stats.vm.v_inactive_count: 113730
-   * vm.stats.vm.v_cache_count: 10809
+   * vm.stats.vm.v_cache_count: 0
+   * vm.stats.vm.v_laundry_count: 10809
    */
-  const char *sysctl_keys[8] = {
+  const char *sysctl_keys[9] = {
       "vm.stats.vm.v_page_size",    "vm.stats.vm.v_page_count",
       "vm.stats.vm.v_free_count",   "vm.stats.vm.v_wire_count",
       "vm.stats.vm.v_active_count", "vm.stats.vm.v_inactive_count",
-      "vm.stats.vm.v_cache_count",  NULL};
-  double sysctl_vals[8];
+      "vm.stats.vm.v_cache_count",  "vm.stats.vm.v_laundry_count",
+      NULL};
+  double sysctl_vals[9];
 
   for (int i = 0; sysctl_keys[i] != NULL; i++) {
     int value;
@@ -258,7 +260,7 @@ static int memory_read_internal(value_list_t *vl) {
   MEMORY_SUBMIT("free", (gauge_t)sysctl_vals[2], "wired",
                 (gauge_t)sysctl_vals[3], "active", (gauge_t)sysctl_vals[4],
                 "inactive", (gauge_t)sysctl_vals[5], "cache",
-                (gauge_t)sysctl_vals[6]);
+                (gauge_t)sysctl_vals[6], "laundry", (gauge_t)sysctl_vals[7]);
 /* #endif HAVE_SYSCTLBYNAME */
 
 #elif KERNEL_LINUX
