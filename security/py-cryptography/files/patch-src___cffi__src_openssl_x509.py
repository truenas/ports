--- src/_cffi_src/openssl/x509.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/x509.py
@@ -340,7 +340,7 @@ void X509_REQ_get0_signature(const X509_REQ *, const A
 CUSTOMIZATIONS = """
 /* Added in 1.0.2 beta but we need it in all versions now due to the great
    opaquing. */
-#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_102
+#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_102 && !CRYPTOGRAPHY_LIBRESSL_27_OR_GREATER
 /* from x509/x_x509.c version 1.0.2 */
 void X509_get0_signature(ASN1_BIT_STRING **psig, X509_ALGOR **palg,
                          const X509 *x)
@@ -387,7 +387,17 @@ X509_REVOKED *Cryptography_X509_REVOKED_dup(X509_REVOK
 /* Added in 1.1.0 but we need it in all versions now due to the great
    opaquing. */
 #if CRYPTOGRAPHY_OPENSSL_LESS_THAN_110
+int i2d_re_X509_REQ_tbs(X509_REQ *req, unsigned char **pp)
+{
+    req->req_info->enc.modified = 1;
+    return i2d_X509_REQ_INFO(req->req_info, pp);
+}
+int i2d_re_X509_CRL_tbs(X509_CRL *crl, unsigned char **pp) {
+    crl->crl->enc.modified = 1;
+    return i2d_X509_CRL_INFO(crl->crl, pp);
+}
 
+#if !CRYPTOGRAPHY_LIBRESSL_27_OR_GREATER
 int X509_up_ref(X509 *x) {
    return CRYPTO_add(&x->references, 1, CRYPTO_LOCK_X509);
 }
@@ -406,16 +416,6 @@ void X509_REQ_get0_signature(const X509_REQ *req, cons
     if (palg != NULL)
         *palg = req->sig_alg;
 }
-int i2d_re_X509_REQ_tbs(X509_REQ *req, unsigned char **pp)
-{
-    req->req_info->enc.modified = 1;
-    return i2d_X509_REQ_INFO(req->req_info, pp);
-}
-int i2d_re_X509_CRL_tbs(X509_CRL *crl, unsigned char **pp) {
-    crl->crl->enc.modified = 1;
-    return i2d_X509_CRL_INFO(crl->crl, pp);
-}
-
 void X509_CRL_get0_signature(const X509_CRL *crl, const ASN1_BIT_STRING **psig,
                              const X509_ALGOR **palg)
 {
@@ -432,5 +432,6 @@ const ASN1_INTEGER *X509_REVOKED_get0_serialNumber(con
 {
     return x->serialNumber;
 }
+#endif
 #endif
 """
