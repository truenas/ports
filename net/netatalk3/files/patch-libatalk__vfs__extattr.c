--- libatalk/vfs/extattr.c.orig	2018-12-19 23:10:29.000000000 -0800
+++ libatalk/vfs/extattr.c	2020-12-29 04:34:36.906199465 -0800
@@ -354,9 +354,9 @@ static ssize_t bsd_attr_list (int type, 
 
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
