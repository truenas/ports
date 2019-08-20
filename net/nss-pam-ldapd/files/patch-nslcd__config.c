--- nslcd/config.c.orig	2019-08-20 09:41:43.876200698 -0700
+++ nslcd/config.c	2019-08-20 09:43:37.370661586 -0700
@@ -32,6 +32,7 @@
 #include "common.h"
 #include "log.h"
 #include "cfg.h"
+#include "myldap.h"
 
 int nslcd_config_get(TFILE *fp, MYLDAP_SESSION UNUSED(*session))
 {
@@ -59,3 +60,23 @@ int nslcd_config_get(TFILE *fp, MYLDAP_S
   WRITE_INT32(fp, NSLCD_RESULT_END);
   return 0;
 }
+
+int nslcd_state_get(TFILE *fp, MYLDAP_SESSION UNUSED(*session))
+{
+  int32_t tmpint32;
+  char* ldap_state = NULL;
+  ldap_state = myldap_dumpstate();
+  if (ldap_state == NULL) {
+    log_log(LOG_WARNING, "Couldn't get ldap state"); 
+    return 0;
+  }
+  /* read request parameters */
+  /* write the response header */
+  WRITE_INT32(fp, NSLCD_VERSION);
+  WRITE_INT32(fp, NSLCD_ACTION_STATE_GET);
+  WRITE_INT32(fp, NSLCD_RESULT_BEGIN);
+  WRITE_STRING(fp,ldap_state);
+  WRITE_INT32(fp, NSLCD_RESULT_END);
+  free(ldap_state);
+  return 0;
+}
