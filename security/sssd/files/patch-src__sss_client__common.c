--- src/sss_client/common.c.orig	2016-04-13 10:48:41.000000000 -0400
+++ src/sss_client/common.c	2018-10-01 05:27:08.141301000 -0400
@@ -25,6 +25,7 @@
 #include "config.h"
 
 #include <nss.h>
+#include <nsswitch.h>
 #include <security/pam_modules.h>
 #include <errno.h>
 #include <sys/types.h>
@@ -43,6 +44,7 @@
 #include <libintl.h>
 #define _(STRING) dgettext (PACKAGE, STRING)
 #include "sss_cli.h"
+#include "util/sss_bsd_errno.h"
 
 #if HAVE_PTHREAD
 #include <pthread.h>
@@ -124,7 +126,6 @@ static enum sss_status sss_cli_send_req(
             *errnop = error;
             break;
         case 0:
-            *errnop = ETIME;
             break;
         case 1:
             if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
@@ -232,7 +233,6 @@ static enum sss_status sss_cli_recv_rep(
             *errnop = error;
             break;
         case 0:
-            *errnop = ETIME;
             break;
         case 1:
             if (pfd.revents & (POLLHUP)) {
@@ -669,7 +669,6 @@ static enum sss_status sss_cli_check_soc
             *errnop = error;
             break;
         case 0:
-            *errnop = ETIME;
             break;
         case 1:
             if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
@@ -719,7 +718,7 @@ enum nss_status sss_nss_make_request(enu
     /* avoid looping in the nss daemon */
     envval = getenv("_SSS_LOOPS");
     if (envval && strcmp(envval, "NO") == 0) {
-        return NSS_STATUS_NOTFOUND;
+	return NS_NOTFOUND;
     }
 
     ret = sss_cli_check_socket(errnop, SSS_NSS_SOCKET_NAME);
@@ -727,9 +726,9 @@ enum nss_status sss_nss_make_request(enu
 #ifdef NONSTANDARD_SSS_NSS_BEHAVIOUR
         *errnop = 0;
         errno = 0;
-        return NSS_STATUS_NOTFOUND;
+        return NS_NOTFOUND;
 #else
-        return NSS_STATUS_UNAVAIL;
+	return NS_UNAVAIL;
 #endif
     }
 
@@ -741,9 +740,9 @@ enum nss_status sss_nss_make_request(enu
 #ifdef NONSTANDARD_SSS_NSS_BEHAVIOUR
             *errnop = 0;
             errno = 0;
-            return NSS_STATUS_NOTFOUND;
+            return NS_NOTFOUND; 
 #else
-            return NSS_STATUS_UNAVAIL;
+            return NS_UNAVAIL;
 #endif
         }
 
@@ -760,9 +759,9 @@ enum nss_status sss_nss_make_request(enu
 #ifdef NONSTANDARD_SSS_NSS_BEHAVIOUR
         *errnop = 0;
         errno = 0;
-        return NSS_STATUS_NOTFOUND;
+        return NS_NOTFOUND;
 #else
-        return NSS_STATUS_UNAVAIL;
+        return NS_UNAVAIL;
 #endif
     }
 }
@@ -791,12 +790,12 @@ int sss_pac_make_request(enum sss_cli_co
     /* avoid looping in the nss daemon */
     envval = getenv("_SSS_LOOPS");
     if (envval && strcmp(envval, "NO") == 0) {
-        return NSS_STATUS_NOTFOUND;
+        return NS_NOTFOUND;
     }
 
     ret = sss_cli_check_socket(errnop, SSS_PAC_SOCKET_NAME);
     if (ret != SSS_STATUS_SUCCESS) {
-        return NSS_STATUS_UNAVAIL;
+        return NS_UNAVAIL;
     }
 
     ret = sss_cli_make_request_nochecks(cmd, rd, repbuf, replen, errnop);
@@ -804,7 +803,7 @@ int sss_pac_make_request(enum sss_cli_co
         /* try reopen socket */
         ret = sss_cli_check_socket(errnop, SSS_PAC_SOCKET_NAME);
         if (ret != SSS_STATUS_SUCCESS) {
-            return NSS_STATUS_UNAVAIL;
+            return NS_UNAVAIL;
         }
 
         /* and make request one more time */
@@ -812,12 +811,12 @@ int sss_pac_make_request(enum sss_cli_co
     }
     switch (ret) {
     case SSS_STATUS_TRYAGAIN:
-        return NSS_STATUS_TRYAGAIN;
+        return NS_TRYAGAIN;
     case SSS_STATUS_SUCCESS:
-        return NSS_STATUS_SUCCESS;
+        return NS_SUCCESS;
     case SSS_STATUS_UNAVAIL:
     default:
-        return NSS_STATUS_UNAVAIL;
+        return NS_UNAVAIL;
     }
 }
 
