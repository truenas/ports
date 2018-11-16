--- src/util/util.h.orig	2016-04-13 10:48:41.000000000 -0400
+++ src/util/util.h	2018-10-02 13:13:45.561374000 -0400
@@ -30,7 +30,7 @@
 #include <string.h>
 #include <strings.h>
 #include <ctype.h>
-#include <errno.h>
+#include <errno.h> 
 #include <libintl.h>
 #include <limits.h>
 #include <locale.h>
@@ -40,6 +40,7 @@
 #include <sys/stat.h>
 #include <arpa/inet.h>
 #include <netinet/in.h>
+#include "util/sss_bsd_errno.h"
 
 #include <talloc.h>
 #include <tevent.h>
@@ -135,6 +136,8 @@ void talloc_log_fn(const char *msg);
 void sss_log(int priority, const char *format, ...) SSS_ATTRIBUTE_PRINTF(2, 3);
 void sss_log_ext(int priority, int facility, const char *format, ...) SSS_ATTRIBUTE_PRINTF(3, 4);
 
+typedef int errno_t;
+
 /* from server.c */
 struct main_context {
     struct tevent_context *event_ctx;
