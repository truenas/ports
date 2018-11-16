--- src/providers/krb5/krb5_delayed_online_authentication.c.orig	2016-04-13 10:48:41.000000000 -0400
+++ src/providers/krb5/krb5_delayed_online_authentication.c	2018-10-01 06:25:27.182445000 -0400
@@ -121,7 +121,6 @@ static void authenticate_user_done(struc
     int ret;
     int pam_status = PAM_SYSTEM_ERR;
     int dp_err = DP_ERR_OK;
-
     ret = krb5_auth_queue_recv(req, &pam_status, &dp_err);
     talloc_free(req);
     if (ret) {
@@ -207,7 +206,6 @@ static errno_t authenticate_stored_users
     }
 
     talloc_free(iter);
-
     return EOK;
 }
 
@@ -321,6 +319,7 @@ errno_t init_delayed_online_authenticati
                                            struct tevent_context *ev)
 {
     int ret;
+#ifdef __linux__
     hash_table_t *tmp_table;
 
     ret = get_uid_table(krb5_ctx, &tmp_table);
@@ -340,7 +339,7 @@ errno_t init_delayed_online_authenticati
               "hash_destroy failed [%s].\n", hash_error_string(ret));
         return EFAULT;
     }
-
+#endif /* __linux__ */
     krb5_ctx->deferred_auth_ctx = talloc_zero(krb5_ctx,
                                           struct deferred_auth_ctx);
     if (krb5_ctx->deferred_auth_ctx == NULL) {
