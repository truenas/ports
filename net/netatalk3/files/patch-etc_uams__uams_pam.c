--- etc/uams/uams_pam.c.orig	2024-03-08 15:31:04.566604000 -0600
+++ etc/uams/uams_pam.c	2024-03-08 15:32:01.276272000 -0600
@@ -53,11 +53,7 @@ extern UAM_MODULE_EXPORT void append(struct papfile *,
  * echo off means password.
  */
 static int PAM_conv (int num_msg,
-#ifdef LINUX
                      const struct pam_message **msg,
-#else
-                     struct pam_message **msg,
-#endif
                      struct pam_response **resp,
                      void *appdata_ptr _U_) 
 {
