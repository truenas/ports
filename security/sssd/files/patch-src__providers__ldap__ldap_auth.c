--- src/providers/ldap/ldap_auth.c.orig	2016-04-13 10:48:41.000000000 -0400
+++ src/providers/ldap/ldap_auth.c	2018-10-01 06:53:49.709291000 -0400
@@ -37,7 +37,6 @@
 #include <sys/time.h>
 #include <strings.h>
 
-#include <shadow.h>
 #include <security/pam_modules.h>
 
 #include "util/util.h"
@@ -51,6 +50,24 @@
 
 #define LDAP_PWEXPIRE_WARNING_TIME 0
 
+
+struct spwd
+{
+  char *sp_namp;              /* Login name.  */
+  char *sp_pwdp;              /* Encrypted password.  */
+  long int sp_lstchg;         /* Date of last change.  */
+  long int sp_min;            /* Minimum number of days between changes.  */
+  long int sp_max;            /* Maximum number of days between changes.  */
+  long int sp_warn;           /* Number of days to warn user to change
+				 the password.  */
+  long int sp_inact;          /* Number of days the account may be
+				 inactive.  */
+  long int sp_expire;         /* Number of days since 1970-01-01 until
+				 account expires.  */
+  unsigned long int sp_flag;  /* Reserved.  */
+};
+
+
 static errno_t add_expired_warning(struct pam_data *pd, long exp_time)
 {
     int ret;
@@ -96,9 +113,9 @@ static errno_t check_pwexpire_kerberos(c
     }
 
     DEBUG(SSSDBG_TRACE_ALL,
-          "Time info: tzname[0] [%s] tzname[1] [%s] timezone [%ld] "
-           "daylight [%d] now [%ld] expire_time [%ld].\n", tzname[0],
-           tzname[1], timezone, daylight, now, expire_time);
+          "Time info: tzname[0] [%s] tzname[1] [%s]"
+           "now [%ld] expire_time [%ld].\n", tzname[0],
+           tzname[1], now, expire_time);
 
     if (difftime(now, expire_time) > 0.0) {
         DEBUG(SSSDBG_CONF_SETTINGS, "Kerberos password expired.\n");
@@ -945,7 +962,7 @@ void sdap_pam_chpass_handler(struct be_r
     DEBUG(SSSDBG_OP_FAILURE,
           "starting password change request for user [%s].\n", pd->user);
 
-    pd->pam_status = PAM_SYSTEM_ERR;
+    pd->pam_status = PAM_SERVICE_ERR;
 
     if (pd->cmd != SSS_PAM_CHAUTHTOK && pd->cmd != SSS_PAM_CHAUTHTOK_PRELIM) {
         DEBUG(SSSDBG_OP_FAILURE,
@@ -1012,7 +1029,7 @@ static void sdap_auth4chpass_done(struct
                 DEBUG(SSSDBG_CRIT_FAILURE,
                       "LDAP provider cannot change kerberos "
                           "passwords.\n");
-                state->pd->pam_status = PAM_SYSTEM_ERR;
+                state->pd->pam_status = PAM_SERVICE_ERR;
                 goto done;
             }
             break;
@@ -1021,7 +1038,7 @@ static void sdap_auth4chpass_done(struct
             break;
         default:
             DEBUG(SSSDBG_CRIT_FAILURE, "Unknown password expiration type.\n");
-                state->pd->pam_status = PAM_SYSTEM_ERR;
+                state->pd->pam_status = PAM_SERVICE_ERR;
                 goto done;
         }
     }
@@ -1045,13 +1062,13 @@ static void sdap_auth4chpass_done(struct
             ret = sss_authtok_get_password(state->pd->authtok,
                                            &password, NULL);
             if (ret) {
-                state->pd->pam_status = PAM_SYSTEM_ERR;
+                state->pd->pam_status = PAM_SERVICE_ERR;
                 goto done;
             }
             ret = sss_authtok_get_password(state->pd->newauthtok,
                                            &new_password, NULL);
             if (ret) {
-                state->pd->pam_status = PAM_SYSTEM_ERR;
+                state->pd->pam_status = PAM_SERVICE_ERR;
                 goto done;
             }
 
@@ -1094,7 +1111,7 @@ static void sdap_auth4chpass_done(struct
         dp_err = DP_ERR_OFFLINE;
         break;
     default:
-        state->pd->pam_status = PAM_SYSTEM_ERR;
+        state->pd->pam_status = PAM_SERVICE_ERR;
     }
 
 done:
@@ -1129,7 +1146,7 @@ static void sdap_pam_chpass_done(struct 
         state->pd->pam_status = PAM_AUTHTOK_ERR;
         break;
     default:
-        state->pd->pam_status = PAM_SYSTEM_ERR;
+        state->pd->pam_status = PAM_SERVICE_ERR;
         break;
     }
 
@@ -1156,7 +1173,7 @@ static void sdap_pam_chpass_done(struct 
                                                     state->sh, state->dn,
                                                     lastchanged_name);
         if (subreq == NULL) {
-            state->pd->pam_status = PAM_SYSTEM_ERR;
+            state->pd->pam_status = PAM_SERVICE_ERR;
             goto done;
         }
 
@@ -1177,7 +1194,7 @@ static void sdap_lastchange_done(struct 
 
     ret = sdap_modify_shadow_lastchange_recv(req);
     if (ret != EOK) {
-        state->pd->pam_status = PAM_SYSTEM_ERR;
+        state->pd->pam_status = PAM_SERVICE_ERR;
         goto done;
     }
 
@@ -1218,7 +1235,7 @@ void sdap_pam_auth_handler(struct be_req
         goto done;
     }
 
-    pd->pam_status = PAM_SYSTEM_ERR;
+    pd->pam_status = PAM_SERVICE_ERR;
 
     switch (pd->cmd) {
     case SSS_PAM_AUTHENTICATE:
@@ -1277,7 +1294,7 @@ static void sdap_pam_auth_done(struct te
                                     be_ctx->domain->pwd_expiration_warning);
         if (ret == EINVAL) {
             /* Unknown password expiration type. */
-            state->pd->pam_status = PAM_SYSTEM_ERR;
+            state->pd->pam_status = PAM_SERVICE_ERR;
             goto done;
         }
     }
@@ -1307,7 +1324,7 @@ static void sdap_pam_auth_done(struct te
         state->pd->pam_status = PAM_PERM_DENIED;
         break;
     default:
-        state->pd->pam_status = PAM_SYSTEM_ERR;
+        state->pd->pam_status = PAM_SERVICE_ERR;
         dp_err = DP_ERR_FATAL;
     }
 
