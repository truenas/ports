--- src/util/debug.h.orig	2018-10-02 13:16:24.257854000 -0400
+++ src/util/debug.h	2018-10-02 13:16:49.128404000 -0400
@@ -31,6 +31,7 @@
 
 #define APPEND_LINE_FEED 0x1
 
+typedef int errno_t;
 extern const char *debug_prg_name;
 extern int debug_level;
 extern int debug_timestamps;
