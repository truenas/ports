--- src/_cffi_src/openssl/rsa.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/rsa.py
@@ -12,10 +12,8 @@ TYPES = """
 typedef ... RSA;
 typedef ... BN_GENCB;
 static const int RSA_PKCS1_PADDING;
-static const int RSA_SSLV23_PADDING;
 static const int RSA_NO_PADDING;
 static const int RSA_PKCS1_OAEP_PADDING;
-static const int RSA_X931_PADDING;
 static const int RSA_PKCS1_PSS_PADDING;
 static const int RSA_F4;
 
@@ -32,7 +30,6 @@ int RSA_generate_key_ex(RSA *, int, BIGNUM *, BN_GENCB
 int RSA_check_key(const RSA *);
 RSA *RSAPublicKey_dup(RSA *);
 int RSA_blinding_on(RSA *, BN_CTX *);
-void RSA_blinding_off(RSA *);
 int RSA_public_encrypt(int, const unsigned char *, unsigned char *,
                        RSA *, int);
 int RSA_private_encrypt(int, const unsigned char *, unsigned char *,
@@ -42,14 +39,6 @@ int RSA_public_decrypt(int, const unsigned char *, uns
 int RSA_private_decrypt(int, const unsigned char *, unsigned char *,
                         RSA *, int);
 int RSA_print(BIO *, const RSA *, int);
-int RSA_verify_PKCS1_PSS(RSA *, const unsigned char *, const EVP_MD *,
-                         const unsigned char *, int);
-int RSA_padding_add_PKCS1_PSS(RSA *, unsigned char *, const unsigned char *,
-                              const EVP_MD *, int);
-int RSA_padding_add_PKCS1_OAEP(unsigned char *, int, const unsigned char *,
-                               int, const unsigned char *, int);
-int RSA_padding_check_PKCS1_OAEP(unsigned char *, int, const unsigned char *,
-                                 int, int, const unsigned char *, int);
 
 /* added in 1.1.0 when the RSA struct was opaqued */
 int RSA_set0_key(RSA *, BIGNUM *, BIGNUM *, BIGNUM *);
@@ -86,8 +75,8 @@ int (*EVP_PKEY_CTX_set0_rsa_oaep_label)(EVP_PKEY_CTX *
                                         int) = NULL;
 #endif
 
-/* These functions were added in OpenSSL 1.1.0-pre5 (beta2) */
-#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_110PRE5
+/* These functions were added in OpenSSL 1.1.0 */
+#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_110 && !CRYPTOGRAPHY_LIBRESSL_27_OR_GREATER
 int RSA_set0_key(RSA *r, BIGNUM *n, BIGNUM *e, BIGNUM *d)
 {
     /* If the fields n and e in r are NULL, the corresponding input
