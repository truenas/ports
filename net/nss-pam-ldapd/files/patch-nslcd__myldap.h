--- nslcd/myldap.h.orig	2019-08-20 09:59:04.967377942 -0700
+++ nslcd/myldap.h	2019-08-20 09:59:22.719511010 -0700
@@ -40,7 +40,7 @@
 /* for LDAP_SCOPE_* */
 #include <lber.h>
 #include <ldap.h>
-
+#include <jansson.h>
 #include "compat/attrs.h"
 
 #ifndef LDAP_SCOPE_DEFAULT
@@ -92,6 +92,9 @@ void myldap_session_close(MYLDAP_SESSION
    reconnect_sleeptime and reconnect_retrytime sleeping period is cut short. */
 void myldap_immediate_reconnect(void);
 
+/* Dump full URI state in JSON. Allocates memory. */
+char *myldap_dumpstate(void);
+
 /* Do an LDAP search and return a reference to the results (returns NULL on
    error). This function uses paging, and does reconnects to the configured
    URLs transparently. The function returns an LDAP status code in the
