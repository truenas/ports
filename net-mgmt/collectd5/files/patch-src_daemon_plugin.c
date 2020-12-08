--- src/daemon/plugin.c.orig
+++ src/daemon/plugin.c
@@ -161,6 +161,8 @@ static pthread_mutex_t statistics_lock = PTHREAD_MUTEX_INITIALIZER;
 static derive_t stats_values_dropped;
 static bool record_statistics;
 
+static cdtime_t cpu_last_read;
+
 /*
  * Static functions
  */
@@ -506,6 +509,14 @@ static void *plugin_read_thread(void __attribute__((unused)) * args) {
       break;
     }
 
+    if (strcmp(rf->rf_name, "aggregation") == 0) {
+      if (cdtime() - cpu_last_read > 5e9) {
+        rf->rf_next_read += 0.5e9;
+        c_heap_insert(read_heap, rf);
+        continue;
+      }
+    }
+
     /* The entry has been marked for deletion. The linked list
      * entry has already been removed by `plugin_unregister_read'.
      * All we have to do here is free the `read_func_t' and
@@ -596,6 +616,10 @@ static void *plugin_read_thread(void __attribute__((unused)) * args) {
 
     /* Re-insert this read function into the heap again. */
     c_heap_insert(read_heap, rf);
+
+    if (strcmp(rf->rf_name, "cpu") == 0) {
+      cpu_last_read = now;
+    }
   } /* while (read_loop) */
 
   pthread_exit(NULL);
@@ -1642,6 +1666,8 @@ EXPORT int plugin_init_all(void) {
     plugin_register_read("collectd", plugin_update_internal_statistics);
   }
 
+  cpu_last_read = 0;
+
   chain_name = global_option_get("PreCacheChain");
   pre_cache_chain = fc_chain_get_by_name(chain_name);
 
