* Prepend the path of the SSL libraries used for building so the same libraries are
* found and loaded at runtime. Normal search finds base SSL libraries before ports.
*
--- src/network/ssl/qsslsocket_openssl_symbols.cpp.orig	2015-05-07 14:14:44 UTC
+++ src/network/ssl/qsslsocket_openssl_symbols.cpp
@@ -287,6 +287,24 @@ DEFINEFUNC(void, OPENSSL_add_all_algorit
 DEFINEFUNC3(int, SSL_CTX_load_verify_locations, SSL_CTX *ctx, ctx, const char *CAfile, CAfile, const char *CApath, CApath, return 0, return)
 DEFINEFUNC(long, SSLeay, void, DUMMYARG, return 0, return)
 
+// Obtained from Arch Linux,
+//  https://git.archlinux.org/svntogit/packages.git/tree/trunk/qt4-openssl-1.1.patch?h=packages/qt4
+DEFINEFUNC(X509_STORE *, SSL_CTX_get_cert_store, const SSL_CTX *ctx, ctx, return 0, return)
+DEFINEFUNC(ASN1_INTEGER *, X509_get_serialNumber, X509 *x, x, return 0, return)
+#if OPENSSL_VERSION_NUMBER >= 0x10100000L
+DEFINEFUNC(int, EVP_PKEY_id, const EVP_PKEY *pkey, pkey, return 0, return)
+DEFINEFUNC(int, EVP_PKEY_base_id, const EVP_PKEY *pkey, pkey, return 0, return)
+DEFINEFUNC2(int, SSL_CIPHER_get_bits, const SSL_CIPHER *cipher, cipher, int *alg_bits, alg_bits, return 0, return)
+DEFINEFUNC2(long, SSL_CTX_set_options, SSL_CTX *ctx, ctx, long options, options, return 0, return)
+DEFINEFUNC(long, X509_get_version, X509 *x, x, return 0, return)
+DEFINEFUNC(X509_PUBKEY *, X509_get_X509_PUBKEY, X509 *x, x, return 0, return)
+DEFINEFUNC(int, RSA_bits,  const RSA *rsa, rsa, return 0, return)
+DEFINEFUNC(int, DSA_security_bits, const DSA *dsa, dsa, return 0, return)
+DEFINEFUNC(const ASN1_TIME *, X509_get0_notAfter, X509 *x, x, return 0, return)
+DEFINEFUNC(const ASN1_TIME *, X509_get0_notBefore, X509 *x, x, return 0, return)
+DEFINEFUNC4(void, DSA_get0_pqg, const DSA *d, d, BIGNUM **p, p, BIGNUM **q, q, BIGNUM **g, g, return, return)
+#endif
+
 #ifdef Q_OS_SYMBIAN
 #define RESOLVEFUNC(func, ordinal, lib) \
     if (!(_q_##func = _q_PTR_##func(lib->resolve(#ordinal)))) \
@@ -511,9 +529,9 @@ static QPair<QLibrary*, QLibrary*> loadO
     libcrypto->setLoadHints(libcrypto->loadHints() | QLibrary::ImprovedSearchHeuristics);
 #elif defined(SHLIB_VERSION_NUMBER)
     // first attempt: the canonical name is libssl.so.<SHLIB_VERSION_NUMBER>
-    libssl->setFileNameAndVersion(QLatin1String("ssl"), QLatin1String(SHLIB_VERSION_NUMBER));
+    libssl->setFileNameAndVersion(QLatin1String("%%OPENSSLLIB%%/libssl"), QLatin1String(SHLIB_VERSION_NUMBER));
     libssl->setLoadHints(QLibrary::ImprovedSearchHeuristics);
-    libcrypto->setFileNameAndVersion(QLatin1String("crypto"), QLatin1String(SHLIB_VERSION_NUMBER));
+    libcrypto->setFileNameAndVersion(QLatin1String("%%OPENSSLLIB%%/libcrypto"), QLatin1String(SHLIB_VERSION_NUMBER));
     libcrypto->setLoadHints(libcrypto->loadHints() | QLibrary::ImprovedSearchHeuristics);
     if (libcrypto->load() && libssl->load()) {
         // libssl.so.<SHLIB_VERSION_NUMBER> and libcrypto.so.<SHLIB_VERSION_NUMBER> found
@@ -525,8 +543,8 @@ static QPair<QLibrary*, QLibrary*> loadO
 #endif
 
     // second attempt: find the development files libssl.so and libcrypto.so
-    libssl->setFileNameAndVersion(QLatin1String("ssl"), -1);
-    libcrypto->setFileNameAndVersion(QLatin1String("crypto"), -1);
+    libssl->setFileNameAndVersion(QLatin1String("%%OPENSSLLIB%%/libssl"), -1);
+    libcrypto->setFileNameAndVersion(QLatin1String("%%OPENSSLLIB%%/libcrypto"), -1);
     if (libcrypto->load() && libssl->load()) {
         // libssl.so.0 and libcrypto.so.0 found
         return pair;
@@ -871,6 +889,26 @@ bool q_resolveOpenSslSymbols()
     RESOLVEFUNC(OPENSSL_add_all_algorithms_conf)
     RESOLVEFUNC(SSL_CTX_load_verify_locations)
     RESOLVEFUNC(SSLeay)
+
+    // Obtained from Arch Linux,
+    //  https://git.archlinux.org/svntogit/packages.git/tree/trunk/qt4-openssl-1.1.patch?h=packages/qt4
+    RESOLVEFUNC(SSL_CTX_get_cert_store)
+    RESOLVEFUNC(X509_get_serialNumber)
+#if OPENSSL_VERSION_NUMBER >= 0x10100000L
+    RESOLVEFUNC(SSL_CTX_ctrl)
+    RESOLVEFUNC(EVP_PKEY_id)
+    RESOLVEFUNC(EVP_PKEY_base_id)
+    RESOLVEFUNC(SSL_CIPHER_get_bits)
+    RESOLVEFUNC(SSL_CTX_set_options)
+    RESOLVEFUNC(X509_get_version)
+    RESOLVEFUNC(X509_get_X509_PUBKEY)
+    RESOLVEFUNC(RSA_bits)
+    RESOLVEFUNC(DSA_security_bits)
+    RESOLVEFUNC(DSA_get0_pqg)
+    RESOLVEFUNC(X509_get0_notAfter)
+    RESOLVEFUNC(X509_get0_notBefore)
+#endif
+
 #endif // Q_OS_SYMBIAN
     symbolsResolved = true;
     delete libs.first;
