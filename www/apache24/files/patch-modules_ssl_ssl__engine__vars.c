--- modules/ssl/ssl_engine_vars.c.orig	2018-04-04 20:36:15 UTC
+++ modules/ssl/ssl_engine_vars.c
@@ -529,7 +529,7 @@ static char *ssl_var_lookup_ssl_cert(apr_pool_t *p, re
         resdup = FALSE;
     }
     else if (strcEQ(var, "A_SIG")) {
-#if MODSSL_USE_OPENSSL_PRE_1_1_API
+#if MODSSL_USE_OPENSSL_PRE_1_1_API || defined(LIBRESSL_VERSION_NUMBER)
         nid = OBJ_obj2nid((ASN1_OBJECT *)(xs->cert_info->signature->algorithm));
 #else
         const ASN1_OBJECT *paobj;
