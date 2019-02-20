--- cipher.c.orig	2018-10-16 17:01:20.000000000 -0700
+++ cipher.c	2018-11-10 11:35:07.816193000 -0800
@@ -67,6 +67,7 @@ struct sshcipher {
 	u_int	key_len;
 	u_int	iv_len;		/* defaults to block_size */
 	u_int	auth_len;
+	u_int	discard_len;
 	u_int	flags;
 #define CFLAG_CBC		(1<<0)
 #define CFLAG_CHACHAPOLY	(1<<1)
@@ -83,32 +84,40 @@ struct sshcipher {
 static const struct sshcipher ciphers[] = {
 #ifdef WITH_OPENSSL
 #ifndef OPENSSL_NO_DES
-	{ "3des-cbc",		8, 24, 0, 0, CFLAG_CBC, EVP_des_ede3_cbc },
+	{ "3des-cbc",		8, 24, 0, 0, 0, CFLAG_CBC, EVP_des_ede3_cbc },
 #endif
-	{ "aes128-cbc",		16, 16, 0, 0, CFLAG_CBC, EVP_aes_128_cbc },
-	{ "aes192-cbc",		16, 24, 0, 0, CFLAG_CBC, EVP_aes_192_cbc },
-	{ "aes256-cbc",		16, 32, 0, 0, CFLAG_CBC, EVP_aes_256_cbc },
+# ifndef OPENSSL_NO_BF
+	{ "blowfish-cbc",	8, 16, 0, 0, 0, 1, EVP_bf_cbc },
+# endif /* OPENSSL_NO_BF */
+# ifndef OPENSSL_NO_CAST
+	{ "cast128-cbc",	8, 16, 0, 0, 0, 1, EVP_cast5_cbc },
+# endif /* OPENSSL_NO_CAST */
+# ifndef OPENSSL_NO_RC4
+	{ "arcfour",		8, 16, 0, 0, 0, 0, EVP_rc4 },
+	{ "arcfour128",		8, 16, 0, 0, 1536, 0, EVP_rc4 },
+	{ "arcfour256",		8, 32, 0, 0, 1536, 0, EVP_rc4 },
+# endif /* OPENSSL_NO_RC4 */
 	{ "rijndael-cbc@lysator.liu.se",
-				16, 32, 0, 0, CFLAG_CBC, EVP_aes_256_cbc },
-	{ "aes128-ctr",		16, 16, 0, 0, 0, EVP_aes_128_ctr },
-	{ "aes192-ctr",		16, 24, 0, 0, 0, EVP_aes_192_ctr },
-	{ "aes256-ctr",		16, 32, 0, 0, 0, EVP_aes_256_ctr },
+				16, 32, 0, 0, 0, 1, EVP_aes_256_cbc },
+	{ "aes128-ctr",		16, 16, 0, 0, 0, 0, EVP_aes_128_ctr },
+	{ "aes192-ctr",		16, 24, 0, 0, 0, 0, EVP_aes_192_ctr },
+	{ "aes256-ctr",		16, 32, 0, 0, 0, 0, EVP_aes_256_ctr },
 # ifdef OPENSSL_HAVE_EVPGCM
 	{ "aes128-gcm@openssh.com",
-				16, 16, 12, 16, 0, EVP_aes_128_gcm },
+				16, 16, 12, 16, 0, 0, EVP_aes_128_gcm },
 	{ "aes256-gcm@openssh.com",
-				16, 32, 12, 16, 0, EVP_aes_256_gcm },
+				16, 32, 12, 16, 0, 0, EVP_aes_256_gcm },
 # endif /* OPENSSL_HAVE_EVPGCM */
 #else
-	{ "aes128-ctr",		16, 16, 0, 0, CFLAG_AESCTR, NULL },
-	{ "aes192-ctr",		16, 24, 0, 0, CFLAG_AESCTR, NULL },
-	{ "aes256-ctr",		16, 32, 0, 0, CFLAG_AESCTR, NULL },
+	{ "aes128-ctr",		16, 16, 0, 0, 0, CFLAG_AESCTR, NULL },
+	{ "aes192-ctr",		16, 24, 0, 0, 0, CFLAG_AESCTR, NULL },
+	{ "aes256-ctr",		16, 32, 0, 0, 0, CFLAG_AESCTR, NULL },
 #endif
 	{ "chacha20-poly1305@openssh.com",
-				8, 64, 0, 16, CFLAG_CHACHAPOLY, NULL },
-	{ "none",		8, 0, 0, 0, CFLAG_NONE, NULL },
+				8, 64, 0, 16, 0, CFLAG_CHACHAPOLY, NULL },
+	{ "none",		8, 0, 0, 0, 0, CFLAG_NONE, NULL },
 
-	{ NULL,			0, 0, 0, 0, 0, NULL }
+	{ NULL,			0, 0, 0, 0, 0, 0, NULL }
 };
 
 /*--*/
@@ -242,6 +251,7 @@ cipher_init(struct sshcipher_ctx **ccp, const struct sshcipher *cipher,
 #ifdef WITH_OPENSSL
 	const EVP_CIPHER *type;
 	int klen;
+	u_char *junk, *discard;
 #endif
 
 	*ccp = NULL;
@@ -303,6 +313,23 @@ cipher_init(struct sshcipher_ctx **ccp, const struct sshcipher *cipher,
 		ret = SSH_ERR_LIBCRYPTO_ERROR;
 		goto out;
 	}
+
+	if (cipher->discard_len > 0) {
+		if ((junk = malloc(cipher->discard_len)) == NULL ||
+		    (discard = malloc(cipher->discard_len)) == NULL) {
+			free(junk);
+			ret = SSH_ERR_ALLOC_FAIL;
+			goto out;
+		}
+		ret = EVP_Cipher(cc->evp, discard, junk, cipher->discard_len);
+		explicit_bzero(discard, cipher->discard_len);
+		free(junk);
+		free(discard);
+		if (ret != 1) {
+			ret = SSH_ERR_LIBCRYPTO_ERROR;
+			goto out;
+		}
+	}
 	ret = 0;
 #endif /* WITH_OPENSSL */
  out:
@@ -552,3 +552,41 @@ cipher_set_keyiv(struct sshcipher_ctx *cc, const u_char *iv, size_t len)
 #endif
 	return 0;
 }
+
+#ifdef WITH_OPENSSL
+#define EVP_X_STATE(evp)	(evp)->cipher_data
+#define EVP_X_STATE_LEN(evp)	(evp)->cipher->ctx_size
+#endif
+
+int
+cipher_get_keycontext(const struct sshcipher_ctx *cc, u_char *dat)
+{
+#if defined(WITH_OPENSSL) && !defined(OPENSSL_NO_RC4)
+	const struct sshcipher *c = cc->cipher;
+	int plen = 0;
+
+	if (c->evptype == EVP_rc4) {
+		plen = EVP_X_STATE_LEN(cc->evp);
+		if (dat == NULL)
+			return (plen);
+		memcpy(dat, EVP_X_STATE(cc->evp), plen);
+	}
+	return (plen);
+#else
+	return 0;
+#endif
+}
+
+void
+cipher_set_keycontext(struct sshcipher_ctx *cc, const u_char *dat)
+{
+#if defined(WITH_OPENSSL) && !defined(OPENSSL_NO_RC4)
+	const struct sshcipher *c = cc->cipher;
+	int plen;
+
+	if (c->evptype == EVP_rc4) {
+		plen = EVP_X_STATE_LEN(cc->evp);
+		memcpy(EVP_X_STATE(cc->evp), dat, plen);
+	}
+#endif
+}
