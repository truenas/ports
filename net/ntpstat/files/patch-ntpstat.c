diff -Naur ntpstat.c.orig ntpstat.c
--- ntpstat.c.org	2013-05-24 00:13:51.000000000 +0400
+++ ntpstat.c	2018-08-26 14:35:27.609566611 +0300
@@ -10,7 +10,7 @@
 #include <sys/socket.h>
 #include <arpa/inet.h>
 #include <sys/time.h>
-#include <error.h>
+#include <netinet/in.h>
 #define NTP_PORT  123
 
 /* This program uses an NTP mode 6 control message, which is the
