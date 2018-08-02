--- src/sss_client/pam_sss.c.orig	2014-09-17 13:01:37 UTC
+++ src/sss_client/pam_sss.c
@@ -1064,7 +1064,7 @@ static int get_pam_items(pam_handle_t *pamh, struct pa
 
     pi->cli_pid = getpid();
 
-    pi->login_name = pam_modutil_getlogin(pamh);
+    pi->login_name = getlogin();
     if (pi->login_name == NULL) pi->login_name="";
 
     pi->domain_name = NULL;
@@ -1145,17 +1145,19 @@ static int send_and_receive(pam_handle_t *pamh, struct
 
     switch (task) {
         case SSS_PAM_AUTHENTICATE:
-            logger(pamh, (pam_status == PAM_SUCCESS ? LOG_INFO : LOG_NOTICE),
-                   "authentication %s; logname=%s uid=%lu euid=%d tty=%s "
-                   "ruser=%s rhost=%s user=%s",
-                   pam_status == PAM_SUCCESS ? "success" : "failure",
-                   pi->login_name, getuid(), (unsigned long) geteuid(),
-                   pi->pam_tty, pi->pam_ruser, pi->pam_rhost, pi->pam_user);
+            if (!quiet_mode || pam_status != PAM_USER_UNKNOWN) {
+                logger(pamh, (pam_status == PAM_SUCCESS ? LOG_INFO : LOG_NOTICE),
+                       "authentication %s; logname=%s uid=%lu euid=%d tty=%s "
+                       "ruser=%s rhost=%s user=%s",
+                       pam_status == PAM_SUCCESS ? "success" : "failure",
+                       pi->login_name, getuid(), (unsigned long) geteuid(),
+                       pi->pam_tty, pi->pam_ruser, pi->pam_rhost, pi->pam_user);
+	    }
             if (pam_status != PAM_SUCCESS) {
                 /* don't log if quiet_mode is on and pam_status is
                  * User not known to the underlying authentication module
                  */
-                if (!quiet_mode || pam_status != 10) {
+                if (!quiet_mode || pam_status != PAM_USER_UNKNOWN) {
                    logger(pamh, LOG_NOTICE, "received for user %s: %d (%s)",
                           pi->pam_user, pam_status,
                           pam_strerror(pamh,pam_status));
@@ -1167,7 +1169,7 @@ static int send_and_receive(pam_handle_t *pamh, struct
                 /* don't log if quiet_mode is on and pam_status is
                  * User not known to the underlying authentication module
                  */
-                if (!quiet_mode || pam_status != 10) {
+                if (!quiet_mode || pam_status != PAM_USER_UNKNOWN) {
                    logger(pamh, LOG_NOTICE,
                           "Authentication failed for user %s: %d (%s)",
                           pi->pam_user, pam_status,
@@ -1188,7 +1190,7 @@ static int send_and_receive(pam_handle_t *pamh, struct
                 /* don't log if quiet_mode is on and pam_status is
                  * User not known to the underlying authentication module
                  */
-                if (!quiet_mode || pam_status != 10) {
+                if (!quiet_mode || pam_status != PAM_USER_UNKNOWN) {
                    logger(pamh, LOG_NOTICE,
                           "Access denied for user %s: %d (%s)",
                           pi->pam_user, pam_status,
