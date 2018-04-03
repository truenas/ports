--- lib/dns/opensslrsa_link.c.orig	2018-03-25 00:18:28 UTC
+++ lib/dns/opensslrsa_link.c
@@ -121,7 +121,7 @@
 #endif
 #define DST_RET(a) {ret = a; goto err;}
 
-#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
+#if OPENSSL_VERSION_NUMBER < 0x10100000L || ( defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L )
 /* From OpenSSL 1.1.0 */
 static int
 RSA_set0_key(RSA *r, BIGNUM *n, BIGNUM *e, BIGNUM *d) {
--- lib/dns/opensslrsa_link.c.orig	2018-03-25 00:18:28 UTC
+++ lib/dns/opensslrsa_link.c
@@ -121,7 +121,7 @@
 #endif
 #define DST_RET(a) {ret = a; goto err;}
 
-#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
+#if OPENSSL_VERSION_NUMBER < 0x10100000L || ( defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L )
 /* From OpenSSL 1.1.0 */
 static int
 RSA_set0_key(RSA *r, BIGNUM *n, BIGNUM *e, BIGNUM *d) {
