--- src/options.c.orig	2017-11-15 07:06:12 UTC
+++ src/options.c
@@ -3658,7 +3658,7 @@ NOEXPORT char *engine_init(void) {
     }
 #endif
     /* engines can add new algorithms */
-#if OPENSSL_VERSION_NUMBER>=0x10100000L
+#if OPENSSL_VERSION_NUMBER>=0x10100000L && !defined(LIBRESSL_VERSION_NUMBER)
     OPENSSL_init_crypto(OPENSSL_INIT_ADD_ALL_CIPHERS|
         OPENSSL_INIT_ADD_ALL_DIGESTS, NULL);
 #else
