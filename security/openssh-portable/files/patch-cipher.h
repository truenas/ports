--- cipher.h.orig	2018-10-16 17:01:20.000000000 -0700
+++ cipher.h	2018-11-10 11:35:07.816193000 -0800
@@ -71,5 +71,7 @@ u_int	 cipher_ctx_is_plaintext(struct sshcipher_ctx *);
 int	 cipher_get_keyiv(struct sshcipher_ctx *, u_char *, size_t);
 int	 cipher_set_keyiv(struct sshcipher_ctx *, const u_char *, size_t);
 int	 cipher_get_keyiv_len(const struct sshcipher_ctx *);
+int	 cipher_get_keycontext(const struct sshcipher_ctx *, u_char *);
+void	 cipher_set_keycontext(struct sshcipher_ctx *, const u_char *);
 
 #endif				/* CIPHER_H */
