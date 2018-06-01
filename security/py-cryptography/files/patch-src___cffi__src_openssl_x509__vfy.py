--- src/_cffi_src/openssl/x509_vfy.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/x509_vfy.py
@@ -246,6 +246,7 @@ static const long X509_V_FLAG_SUITEB_128_LOS_ONLY = 0;
 static const long X509_V_FLAG_SUITEB_192_LOS = 0;
 static const long X509_V_FLAG_SUITEB_128_LOS = 0;
 
+#if !CRYPTOGRAPHY_LIBRESSL_27_OR_GREATER
 int (*X509_VERIFY_PARAM_set1_host)(X509_VERIFY_PARAM *, const char *,
                                    size_t) = NULL;
 int (*X509_VERIFY_PARAM_set1_email)(X509_VERIFY_PARAM *, const char *,
@@ -256,6 +257,7 @@ int (*X509_VERIFY_PARAM_set1_ip_asc)(X509_VERIFY_PARAM
 void (*X509_VERIFY_PARAM_set_hostflags)(X509_VERIFY_PARAM *,
                                         unsigned int) = NULL;
 #endif
+#endif
 
 /* OpenSSL 1.0.2+ or Solaris's backport */
 #ifdef X509_V_FLAG_PARTIAL_CHAIN
@@ -273,7 +275,7 @@ static const long Cryptography_HAS_X509_V_FLAG_TRUSTED
 static const long X509_V_FLAG_TRUSTED_FIRST = 0;
 #endif
 
-#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_110PRE6
+#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_110 && !CRYPTOGRAPHY_LIBRESSL_27_OR_GREATER
 Cryptography_STACK_OF_X509_OBJECT *X509_STORE_get0_objects(X509_STORE *ctx) {
     return ctx->objs;
 }
@@ -283,9 +285,7 @@ X509_VERIFY_PARAM *X509_STORE_get0_param(X509_STORE *s
 int X509_OBJECT_get_type(const X509_OBJECT *x) {
     return x->type;
 }
-#endif
 
-#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_110PRE5
 /* from x509/x509_vfy.c */
 X509 *X509_STORE_CTX_get0_cert(X509_STORE_CTX *ctx)
 {
