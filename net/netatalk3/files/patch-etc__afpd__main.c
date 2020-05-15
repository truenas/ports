--- etc/afpd/main.c.orig	2016-03-10 00:06:20.000000000 -0800
+++ etc/afpd/main.c	2020-05-15 09:35:31.435513693 -0700
@@ -11,6 +11,7 @@
 #include <stdlib.h>
 #include <string.h>
 #include <signal.h>
+#include <unistd.h>
 #include <sys/param.h>
 #include <sys/uio.h>
 #include <sys/time.h>
@@ -40,6 +41,8 @@
 #include "afpstats.h"
 
 #define ASEV_THRESHHOLD 10
+#define AFPD_PIDFILE "/var/run/afpd.pid"
+#define PIDDIR "/var/run"
 
 unsigned char nologin = 0;
 
@@ -53,6 +56,7 @@ static afp_child_t *dsi_start(AFPObj *obj, DSI *dsi, s
 
 static void afp_exit(int ret)
 {
+    pidfile_unlink(PIDDIR, "afpd");
     exit(ret);
 }
 
@@ -190,7 +194,7 @@ int main(int ac, char **av)
 {
     struct sigaction	sv;
     sigset_t            sigs;
-    int                 ret;
+    int                 ret, pidfile_fd;
 
     /* Parse argv args and initialize default options */
     afp_options_parse_cmdline(&obj, ac, av);
@@ -350,6 +354,10 @@ int main(int ac, char **av)
      * connection is made without an actual login attempt being made
      * afterwards. establishing timeouts for logins is a possible 
      * solution. */
+    if (access(AFPD_PIDFILE, F_OK) == 0) {
+        pidfile_unlink(PIDDIR, "afpd");
+    }
+    pidfile_create(PIDDIR, "afpd");
     while (1) {
         pthread_sigmask(SIG_UNBLOCK, &sigs, NULL);
         ret = poll(asev->fdset, asev->used, -1);
@@ -456,6 +464,7 @@ int main(int ac, char **av)
         } /* for (i)*/
     } /* while (1) */
 
+    pidfile_unlink(PIDDIR, "afpd");
     return 0;
 }
 
