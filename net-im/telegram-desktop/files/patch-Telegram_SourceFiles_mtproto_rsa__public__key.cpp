--- Telegram/SourceFiles/mtproto/rsa_public_key.cpp.orig	2018-05-15 00:21:21 UTC
+++ Telegram/SourceFiles/mtproto/rsa_public_key.cpp
@@ -16,7 +16,7 @@ https://github.com/telegramdesktop/tdesktop/blob/maste
 namespace MTP {
 namespace internal {
 namespace {
-#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
+#if OPENSSL_VERSION_NUMBER < 0x10100000L || ! defined(LIBRESSL_VERSION_NUMBER)
 
 // This is a key setter for compatibility with OpenSSL 1.0
 int RSA_set0_key(RSA *r, BIGNUM *n, BIGNUM *e, BIGNUM *d) {
