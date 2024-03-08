--- etc/uams/uams_dhx_pam.c.orig	2024-03-08 15:35:04.435847000 -0600
+++ etc/uams/uams_dhx_pam.c	2024-03-08 15:35:20.590577000 -0600
@@ -80,11 +80,7 @@ static char *PAM_password;
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
