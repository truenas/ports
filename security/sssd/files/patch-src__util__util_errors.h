--- src/util/util_errors.h.orig	2018-10-02 13:28:36.758570000 -0400
+++ src/util/util_errors.h	2018-10-02 13:29:54.644762000 -0400
@@ -26,6 +26,7 @@
 typedef int errno_t;
 #endif
 
+typedef int errno_t;
 /*
  * We define a specific number space so that we do not overlap with other
  * generic errors returned by various libraries. This will make it easy
