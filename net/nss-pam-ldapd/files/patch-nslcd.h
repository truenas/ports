--- nslcd.h.orig	2019-08-20 09:22:31.937452244 -0700
+++ nslcd.h	2019-08-20 09:22:44.449564284 -0700
@@ -74,6 +74,7 @@
   the result value is:
     STRING  value, interpretation depending on request */
 #define NSLCD_ACTION_CONFIG_GET        0x00010001
+#define NSLCD_ACTION_STATE_GET         0x00010002
 
 /* return the message, if any, that is presented to the user when password
    modification through PAM is prohibited */
