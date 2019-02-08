--- src/rrdcached.c.orig	2017-06-06 20:13:54.693164693 +0200
+++ src/rrdcached.c	2019-01-25 22:45:29.468646783 +0100
@@ -463,8 +463,8 @@ static int rc_write(const data_set_t *ds, const value_list_t *vl,
       /* else: report the error and fail */
     }
 
-    ERROR("rrdcached plugin: rrdc_update (%s, [%s], 1) failed: %s (status=%i)",
-          filename, values_array[0], rrd_get_error(), status);
+    WARNING("rrdcached plugin: rrdc_update (%s, [%s], 1) failed: %s (status=%i)",
+            filename, values_array[0], rrd_get_error(), status);
     return -1;
   }
 
