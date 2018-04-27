--- source/fitz/load-jpx.c.orig	2018-04-27 14:43:55 UTC
+++ source/fitz/load-jpx.c
@@ -445,8 +445,13 @@
 
 #else /* HAVE_LURATECH */
 
-#include <openjpeg.h>
+#ifdef __cplusplus
+extern "C"
+{
+#endif
 
+#include <openjpeg-2.3/openjpeg.h>
+
 struct fz_jpxd_s
 {
 	int width;
@@ -922,6 +927,10 @@
 	*xresp = state.xres;
 	*yresp = state.yres;
 }
+
+#ifdef __cplusplus
+}
+#endif
 
 #endif /* HAVE_LURATECH */
 
