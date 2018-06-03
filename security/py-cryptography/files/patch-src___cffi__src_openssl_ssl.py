--- src/_cffi_src/openssl/ssl.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/ssl.py
@@ -48,6 +48,7 @@ static const long Cryptography_HAS_SSL_OP_NO_TICKET;
 static const long Cryptography_HAS_ALPN;
 static const long Cryptography_HAS_NEXTPROTONEG;
 static const long Cryptography_HAS_SET_CERT_CB;
+static const long Cryptography_HAS_CUSTOM_EXT;
 
 static const long SSL_FILETYPE_PEM;
 static const long SSL_FILETYPE_ASN1;
@@ -251,7 +252,7 @@ void SSL_CTX_set_psk_server_callback(SSL_CTX *,
                                          SSL *,
                                          const char *,
                                          unsigned char *,
-                                         int
+                                         unsigned int
                                      ));
 void SSL_CTX_set_psk_client_callback(SSL_CTX *,
                                      unsigned int (*)(
@@ -305,6 +306,10 @@ int SSL_CTX_set_ex_data(SSL_CTX *, int, void *);
 
 SSL_SESSION *SSL_get_session(const SSL *);
 const unsigned char *SSL_SESSION_get_id(const SSL_SESSION *, unsigned int *);
+long SSL_SESSION_get_time(const SSL_SESSION *);
+long SSL_SESSION_get_timeout(const SSL_SESSION *);
+int SSL_SESSION_has_ticket(const SSL_SESSION *);
+long SSL_SESSION_get_ticket_lifetime_hint(const SSL_SESSION *);
 
 /* not a macro, but older OpenSSLs don't pass the args as const */
 char *SSL_CIPHER_description(const SSL_CIPHER *, char *, int);
@@ -396,7 +401,7 @@ void *SSL_get_ex_data(const SSL *, int);
 void SSL_set_tlsext_host_name(SSL *, char *);
 void SSL_CTX_set_tlsext_servername_callback(
     SSL_CTX *,
-    int (*)(const SSL *, int *, void *));
+    int (*)(SSL *, int *, void *));
 void SSL_CTX_set_tlsext_servername_arg(
     SSL_CTX *, void *);
 
@@ -488,6 +493,35 @@ long Cryptography_DTLSv1_get_timeout(SSL *, time_t *, 
 long DTLSv1_handle_timeout(SSL *);
 long DTLS_set_link_mtu(SSL *, long);
 long DTLS_get_link_min_mtu(SSL *);
+
+/* Custom extensions. */
+typedef int (*custom_ext_add_cb)(SSL *, unsigned int,
+                                 const unsigned char **,
+                                 size_t *, int *,
+                                 void *);
+
+typedef void (*custom_ext_free_cb)(SSL *, unsigned int,
+                                   const unsigned char *,
+                                   void *);
+
+typedef int (*custom_ext_parse_cb)(SSL *, unsigned int,
+                                   const unsigned char *,
+                                   size_t, int *,
+                                   void *);
+
+int SSL_CTX_add_client_custom_ext(SSL_CTX *, unsigned int,
+                                  custom_ext_add_cb,
+                                  custom_ext_free_cb, void *,
+                                  custom_ext_parse_cb,
+                                  void *);
+
+int SSL_CTX_add_server_custom_ext(SSL_CTX *, unsigned int,
+                                  custom_ext_add_cb,
+                                  custom_ext_free_cb, void *,
+                                  custom_ext_parse_cb,
+                                  void *);
+
+int SSL_extension_supported(unsigned int);
 """
 
 CUSTOMIZATIONS = """
@@ -502,7 +536,7 @@ const SSL_METHOD *SSL_CTX_get_ssl_method(SSL_CTX *ctx)
 
 /* Added in 1.1.0 in the great opaquing, but we need to define it for older
    OpenSSLs. Such is our burden. */
-#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_110
+#if CRYPTOGRAPHY_OPENSSL_LESS_THAN_110 && !CRYPTOGRAPHY_LIBRESSL_27_OR_GREATER
 /* from ssl/ssl_lib.c */
 size_t SSL_get_client_random(const SSL *ssl, unsigned char *out, size_t outlen)
 {
@@ -540,6 +574,16 @@ size_t SSL_SESSION_get_master_key(const SSL_SESSION *s
     memcpy(out, session->master_key, outlen);
     return outlen;
 }
+/* from ssl/ssl_sess.c */
+int SSL_SESSION_has_ticket(const SSL_SESSION *s)
+{
+    return (s->tlsext_ticklen > 0) ? 1 : 0;
+}
+/* from ssl/ssl_sess.c */
+unsigned long SSL_SESSION_get_ticket_lifetime_hint(const SSL_SESSION *s)
+{
+    return s->tlsext_tick_lifetime_hint;
+}
 #endif
 
 static const long Cryptography_HAS_SECURE_RENEGOTIATION = 1;
@@ -694,7 +738,7 @@ void (*SSL_CTX_set_psk_server_callback)(SSL_CTX *,
                                             SSL *,
                                             const char *,
                                             unsigned char *,
-                                            int
+                                            unsigned int
                                         )) = NULL;
 void (*SSL_CTX_set_psk_client_callback)(SSL_CTX *,
                                         unsigned int (*)(
@@ -707,5 +751,43 @@ void (*SSL_CTX_set_psk_client_callback)(SSL_CTX *,
                                         )) = NULL;
 #else
 static const long Cryptography_HAS_PSK = 1;
+#endif
+
+/*
+ * Custom extensions were added in 1.0.2. 1.1.1 is adding a more general
+ * SSL_CTX_add_custom_ext function, but we're not binding that yet.
+ */
+#if CRYPTOGRAPHY_OPENSSL_102_OR_GREATER
+static const long Cryptography_HAS_CUSTOM_EXT = 1;
+#else
+static const long Cryptography_HAS_CUSTOM_EXT = 0;
+
+typedef int (*custom_ext_add_cb)(SSL *, unsigned int,
+                                 const unsigned char **,
+                                 size_t *, int *,
+                                 void *);
+
+typedef void (*custom_ext_free_cb)(SSL *, unsigned int,
+                                   const unsigned char *,
+                                   void *);
+
+typedef int (*custom_ext_parse_cb)(SSL *, unsigned int,
+                                   const unsigned char *,
+                                   size_t, int *,
+                                   void *);
+
+int (*SSL_CTX_add_client_custom_ext)(SSL_CTX *, unsigned int,
+                                     custom_ext_add_cb,
+                                     custom_ext_free_cb, void *,
+                                     custom_ext_parse_cb,
+                                     void *) = NULL;
+
+int (*SSL_CTX_add_server_custom_ext)(SSL_CTX *, unsigned int,
+                                     custom_ext_add_cb,
+                                     custom_ext_free_cb, void *,
+                                     custom_ext_parse_cb,
+                                     void *) = NULL;
+
+int (*SSL_extension_supported)(unsigned int) = NULL;
 #endif
 """
