--- src/_cffi_src/openssl/ecdsa.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/ecdsa.py
@@ -24,14 +24,9 @@ void ECDSA_SIG_free(ECDSA_SIG *);
 int i2d_ECDSA_SIG(const ECDSA_SIG *, unsigned char **);
 ECDSA_SIG *d2i_ECDSA_SIG(ECDSA_SIG **s, const unsigned char **, long);
 ECDSA_SIG *ECDSA_do_sign(const unsigned char *, int, EC_KEY *);
-ECDSA_SIG *ECDSA_do_sign_ex(const unsigned char *, int, const BIGNUM *,
-                            const BIGNUM *, EC_KEY *);
 int ECDSA_do_verify(const unsigned char *, int, const ECDSA_SIG *, EC_KEY *);
-int ECDSA_sign_setup(EC_KEY *, BN_CTX *, BIGNUM **, BIGNUM **);
 int ECDSA_sign(int, const unsigned char *, int, unsigned char *,
                unsigned int *, EC_KEY *);
-int ECDSA_sign_ex(int, const unsigned char *, int dgstlen, unsigned char *,
-                  unsigned int *, const BIGNUM *, const BIGNUM *, EC_KEY *);
 int ECDSA_verify(int, const unsigned char *, int, const unsigned char *, int,
                  EC_KEY *);
 int ECDSA_size(const EC_KEY *);
