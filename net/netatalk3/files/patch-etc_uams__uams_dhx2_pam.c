--- etc/uams/uams_dhx2_pam.c.orig	2024-03-08 15:36:40.349239000 -0600
+++ etc/uams/uams_dhx2_pam.c	2024-03-08 15:36:57.295776000 -0600
@@ -136,11 +136,7 @@ error:
  * echo off means password.
  */
 static int PAM_conv (int num_msg,
-#ifdef LINUX
                      const struct pam_message **msg,
-#else
-                     struct pam_message **msg,
-#endif
                      struct pam_response **resp,
                      void *appdata_ptr _U_) {
     int count = 0;
