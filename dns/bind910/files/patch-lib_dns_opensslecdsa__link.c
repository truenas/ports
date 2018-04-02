--- lib/dns/opensslecdsa_link.c.orig	2018-03-25 00:17:52 UTC
+++ lib/dns/opensslecdsa_link.c
@@ -42,7 +42,7 @@
 
 #define DST_RET(a) {ret = a; goto err;}
 
-#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
+#if OPENSSL_VERSION_NUMBER < 0x10100000L || ( defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L )
 /* From OpenSSL 1.1 */
 static void
 ECDSA_SIG_get0(const ECDSA_SIG *sig, const BIGNUM **pr, const BIGNUM **ps) {
--- lib/dns/opensslecdsa_link.c.orig	2018-03-25 00:17:52 UTC
+++ lib/dns/opensslecdsa_link.c
@@ -42,7 +42,7 @@
 
 #define DST_RET(a) {ret = a; goto err;}
 
-#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
+#if OPENSSL_VERSION_NUMBER < 0x10100000L || ( defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L )
 /* From OpenSSL 1.1 */
 static void
 ECDSA_SIG_get0(const ECDSA_SIG *sig, const BIGNUM **pr, const BIGNUM **ps) {
