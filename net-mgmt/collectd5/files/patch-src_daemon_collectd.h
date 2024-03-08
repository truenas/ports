--- src/daemon/collectd.h.orig	2019-06-13 09:13:42 UTC
+++ src/daemon/collectd.h
@@ -213,7 +213,7 @@ typedef int uid_t;
 #endif
 
 #ifndef LOCALSTATEDIR
-#define LOCALSTATEDIR PREFIX "/var"
+#define LOCALSTATEDIR "/var"
 #endif
 
 #ifndef PKGLOCALSTATEDIR
-#define PKGLOCALSTATEDIR PREFIX "/var/lib/" PACKAGE_NAME
+#define PKGLOCALSTATEDIR "/var/db/" PACKAGE_NAME
 #endif
 
 #ifndef PIDFILE
-#define PIDFILE PREFIX "/var/run/" PACKAGE_NAME ".pid"
+#define PIDFILE "/var/run/" PACKAGE_NAME ".pid"
 #endif
 
 #ifndef PLUGINDIR
