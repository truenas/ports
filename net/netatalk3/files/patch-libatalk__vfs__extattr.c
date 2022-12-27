--- libatalk/vfs/extattr.c.orig	2022-03-21 23:50:23.000000000 -0500
+++ libatalk/vfs/extattr.c	2022-12-27 15:52:51.705108000 -0600
@@ -354,12 +354,16 @@ static ssize_t bsd_attr_list (int type, extattr_arg ar
 
     /* Convert from pascal strings to C strings */
     len = list[0];
-    memmove(list, list + 1, list_size);
-
-    for(i = len; i < list_size; ) {
+    memmove(list, list + 1, list_size -1);
+    list[list_size -1] = '\0';
+    for(i = len; i < list_size -1; ) {
         LOG(log_maxdebug, logtype_afpd, "len: %d, i: %d", len, i);
 
         len = list[i];
+        if (i + len >= list_size) {
+            errno = ERANGE;
+            return -1;
+        }
         list[i] = '\0';
         i += len + 1;
     }
