--- nslcd/myldap.c.orig	2017-06-20 13:47:52.000000000 -0700
+++ nslcd/myldap.c	2019-08-20 10:05:35.581088557 -0700
@@ -34,6 +34,7 @@
 */
 
 #include "config.h"
+#include "nslcd_json.h"
 
 #include <stdio.h>
 #include <stdlib.h>
@@ -49,6 +50,7 @@
 #include <ldap.h>
 #ifdef HAVE_LDAP_SSL_H
 #include <ldap_ssl.h>
+#incldue <jansson.h>
 #endif
 #ifdef HAVE_GSSLDAP_H
 #include <gssldap.h>
@@ -2528,3 +2530,45 @@ int myldap_error_message(MYLDAP_SESSION 
     ldap_memfree(msg_diag);
   return LDAP_SUCCESS;
 }
+
+char *myldap_dumpstate(void)
+{
+  int i;
+  char *json;
+  struct json_object jsobj = json_new_object();
+  struct json_object ldap_servers = json_new_object();
+  int reconnect = (int)nslcd_cfg->reconnect_retrytime;
+  if (json_is_invalid(&jsobj)) {
+    log_log(LOG_WARNING, "Error setting up JSON value: %s\n", strerror(errno));
+    goto failure;
+  }
+  json_add_timestamp(&jsobj);
+  json_add_int(&jsobj, "reconnect_retrytime", reconnect);
+  pthread_mutex_lock(&uris_mutex);
+  for (i = 0; i < (NSS_LDAP_CONFIG_MAX_URIS +1); i++)
+  {
+     if (nslcd_cfg->uris[i].uri != NULL)
+     {
+       struct json_object jsobjint = json_new_object(); 
+       int firstfail = (int)nslcd_cfg->uris[i].firstfail;
+       int lastfail = (int)nslcd_cfg->uris[i].lastfail;
+       if (json_add_int(&jsobjint, "firstfail", firstfail) != 0) {
+           json_free(&jsobjint);
+           goto failure;
+       }
+       if (json_add_int(&jsobjint, "lastfail", lastfail) != 0) {
+           json_free(&jsobjint);
+           goto failure;
+       }
+       json_add_object(&ldap_servers, nslcd_cfg->uris[i].uri, &jsobjint);
+     }
+  }
+  pthread_mutex_unlock(&uris_mutex);
+  json_add_object(&jsobj, "uris", &ldap_servers);
+  json = json_to_string(&jsobj);
+  json_free(&jsobj);
+  return json;
+failure:
+  json_free(&jsobj);
+  return NULL;
+}
