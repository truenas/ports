--- nsswitch/pam_winbind.c.orig	2021-03-01 20:51:47.887048000 -0500
+++ nsswitch/pam_winbind.c	2021-03-02 10:01:38.657167000 -0500
@@ -2682,6 +2682,9 @@ static int openpam_convert_error_code(struct pwb_conte
 		    r == PAM_CRED_ERR) {
 			return r;
 		}
+		else if (r == PAM_AUTHINFO_UNAVAIL) {
+			return PAM_CRED_UNAVAIL;
+		}
 		break;
 	case PAM_WINBIND_ACCT_MGMT:
 		if (r == PAM_USER_UNKNOWN ||
