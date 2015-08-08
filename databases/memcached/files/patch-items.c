--- items.c.orig	2015-04-25 13:47:33.000000000 -0700
+++ items.c	2015-08-08 03:21:51.863017537 -0700
@@ -1138,7 +1138,10 @@
     item **head, **tail;
     assert(it->it_flags == 1);
     assert(it->nbytes == 0);
+#if 0
+    /* This comparison is always true, which generates warning in clang */
     assert(it->slabs_clsid < LARGEST_ID);
+#endif
     head = &heads[it->slabs_clsid];
     tail = &tails[it->slabs_clsid];
 
