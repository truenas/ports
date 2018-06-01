--- src/_cffi_src/openssl/dh.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/dh.py
@@ -45,8 +45,8 @@ int Cryptography_i2d_DHxparams_bio(BIO *bp, DH *x);
 """
 
 CUSTOMIZATIONS = """
-/* These functions were added in OpenSSL 1.1.0-pre5 (beta2) */
-#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_110PRE5
+/* These functions were added in OpenSSL 1.1.0 */
+#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_110 && !CRYPTOGRAPHY_LIBRESSL_27_OR_GREATER
 void DH_get0_pqg(const DH *dh,
                  const BIGNUM **p, const BIGNUM **q, const BIGNUM **g)
 {
