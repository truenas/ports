--- lib/dns/openssldh_link.c.orig	2018-03-25 00:15:52 UTC
+++ lib/dns/openssldh_link.c
@@ -69,7 +69,7 @@ static isc_result_t openssldh_todns(cons
 
 static BIGNUM *bn2, *bn768, *bn1024, *bn1536;
 
-#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
+#if OPENSSL_VERSION_NUMBER < 0x10100000L || ( defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L )
 /*
  * DH_get0_key, DH_set0_key, DH_get0_pqg and DH_set0_pqg
  * are from OpenSSL 1.1.0.
