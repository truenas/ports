--- src/sss_client/pam_sss.c.orig	2018-05-15 16:49:50.533741000 -0400
+++ src/sss_client/pam_sss.c	2018-05-15 16:49:55.330836000 -0400
@@ -1155,7 +1155,7 @@
                 /* don't log if quiet_mode is on and pam_status is
                  * User not known to the underlying authentication module
                  */
-                if (!quiet_mode || pam_status != 10) {
+                if (!quiet_mode || pam_status != PAM_USER_UNKNOWN) {
                    logger(pamh, LOG_NOTICE, "received for user %s: %d (%s)",
                           pi->pam_user, pam_status,
                           pam_strerror(pamh,pam_status));
@@ -1167,7 +1167,7 @@
                 /* don't log if quiet_mode is on and pam_status is
                  * User not known to the underlying authentication module
                  */
-                if (!quiet_mode || pam_status != 10) {
+                if (!quiet_mode || pam_status != PAM_USER_UNKNOWN) {
                    logger(pamh, LOG_NOTICE,
                           "Authentication failed for user %s: %d (%s)",
                           pi->pam_user, pam_status,
@@ -1188,7 +1188,7 @@
                 /* don't log if quiet_mode is on and pam_status is
                  * User not known to the underlying authentication module
                  */
-                if (!quiet_mode || pam_status != 10) {
+                if (!quiet_mode || pam_status != PAM_USER_UNKNOWN) {
                    logger(pamh, LOG_NOTICE,
                           "Access denied for user %s: %d (%s)",
                           pi->pam_user, pam_status,
