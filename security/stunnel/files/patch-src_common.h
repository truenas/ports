--- src/common.h.orig	2017-01-02 14:27:26 UTC
+++ src/common.h
@@ -448,7 +448,7 @@ extern char *sys_errlist[];
 #define OPENSSL_NO_TLS1_2
 #endif /* OpenSSL older than 1.0.1 || defined(OPENSSL_NO_TLS1) */
 
-#if OPENSSL_VERSION_NUMBER>=0x10100000L
+#if OPENSSL_VERSION_NUMBER>=0x10100000L && !defined(LIBRESSL_VERSION_NUMBER)
 #ifndef OPENSSL_NO_SSL2
 #define OPENSSL_NO_SSL2
 #endif /* !defined(OPENSSL_NO_SSL2) */
@@ -474,7 +474,7 @@ extern char *sys_errlist[];
 #include <openssl/des.h>
 #ifndef OPENSSL_NO_DH
 #include <openssl/dh.h>
-#if OPENSSL_VERSION_NUMBER<0x10100000L
+#if OPENSSL_VERSION_NUMBER<0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
 int DH_set0_pqg(DH *dh, BIGNUM *p, BIGNUM *q, BIGNUM *g);
 #endif /* OpenSSL older than 1.1.0 */
 #endif /* !defined(OPENSSL_NO_DH) */
--- src/common.h.orig	2016-06-27 07:29:32 UTC
+++ src/common.h
@@ -415,6 +415,10 @@ extern char *sys_errlist[];
 /* opensslv.h requires prior opensslconf.h to include -fips in version string */
 #include <openssl/opensslv.h>
 
+#ifdef LIBRESSL_VERSION_NUMBER
+#define OPENSSL_VERSION_NUMBER 0x1000107fL
+#endif
+
 #if OPENSSL_VERSION_NUMBER<0x0090700fL
 #error OpenSSL 0.9.7 or later is required
 #endif /* OpenSSL older than 0.9.7 */
