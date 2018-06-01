--- src/_cffi_src/openssl/dsa.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/dsa.py
@@ -34,8 +34,8 @@ int DSA_generate_parameters_ex(DSA *, int, unsigned ch
 """
 
 CUSTOMIZATIONS = """
-/* These functions were added in OpenSSL 1.1.0-pre5 (beta2) */
-#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_110PRE5
+/* These functions were added in OpenSSL 1.1.0 */
+#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_110 && !CRYPTOGRAPHY_LIBRESSL_27_OR_GREATER
 void DSA_get0_pqg(const DSA *d,
                   const BIGNUM **p, const BIGNUM **q, const BIGNUM **g)
 {
