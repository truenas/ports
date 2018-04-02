--- lib/dns/openssldsa_link.c.orig	2018-03-25 00:16:57 UTC
+++ lib/dns/openssldsa_link.c
@@ -49,7 +49,7 @@
 
 static isc_result_t openssldsa_todns(const dst_key_t *key, isc_buffer_t *data);
 
-#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
+#if OPENSSL_VERSION_NUMBER < 0x10100000L || ( defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L )
 static void
 DSA_get0_pqg(const DSA *d, const BIGNUM **p, const BIGNUM **q,
 	     const BIGNUM **g)
--- lib/dns/openssldsa_link.c.orig	2018-03-25 00:16:57 UTC
+++ lib/dns/openssldsa_link.c
@@ -49,7 +49,7 @@
 
 static isc_result_t openssldsa_todns(const dst_key_t *key, isc_buffer_t *data);
 
-#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
+#if OPENSSL_VERSION_NUMBER < 0x10100000L || ( defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L )
 static void
 DSA_get0_pqg(const DSA *d, const BIGNUM **p, const BIGNUM **q,
 	     const BIGNUM **g)
