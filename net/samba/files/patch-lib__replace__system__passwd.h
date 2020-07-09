--- lib/replace/system/passwd.h.orig	2020-06-18 05:34:06.834157901 -0700
+++ lib/replace/system/passwd.h	2020-06-18 05:35:06.951913093 -0700
@@ -89,4 +89,8 @@
 #define ULTRIX_AUTH 1
 #endif
 
+#ifndef NSS_BUFLEN_PASSWD
+#define NSS_BUFLEN_PASSWD 1024
+#endif
+
 #endif
