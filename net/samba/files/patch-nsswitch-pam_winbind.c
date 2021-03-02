--- nsswitch/pam_winbind.c.orig	2021-03-01 20:50:46.687752000 -0500
+++ nsswitch/pam_winbind.c	2021-03-01 20:51:00.856219000 -0500
@@ -2678,6 +2678,7 @@ static int openpam_convert_error_code(struct pwb_conte
 	case PAM_WINBIND_SETCRED:
 		if (r == PAM_CRED_UNAVAIL ||
 		    r == PAM_CRED_EXPIRED ||
+		    r == PAM_AUTHINFO_UNAVAIL ||
 		    r == PAM_USER_UNKNOWN ||
 		    r == PAM_CRED_ERR) {
 			return r;
