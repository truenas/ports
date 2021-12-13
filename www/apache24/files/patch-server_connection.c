--- server/connection.c.orig	2021-12-13 01:25:05.263372000 -0500
+++ server/connection.c	2021-12-13 01:26:36.147865000 -0500
@@ -122,10 +122,7 @@ AP_DECLARE(int) ap_start_lingering_close(conn_rec *c)
 {
     apr_socket_t *csd = ap_get_conn_socket(c);
 
-    if (!csd) {
-        return 1;
-    }
-
+    ap_assert(csd != NULL);
     if (ap_prep_lingering_close(c)) {
         return 1;
     }
@@ -154,6 +151,15 @@ AP_DECLARE(void) ap_lingering_close(conn_rec *c)
     apr_size_t nbytes;
     apr_time_t now, timeup = 0;
     apr_socket_t *csd = ap_get_conn_socket(c);
+
+    if (!csd) {
+        /* Be safe with third-party modules that:
+         *   ap_set_core_module_config(c->conn_config, NULL)
+         * to no-op ap_lingering_close().
+         */
+        c->aborted = 1;
+        return;
+    }
 
     if (ap_start_lingering_close(c)) {
         apr_socket_close(csd);
