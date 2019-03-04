--- contrib/mod_tls.c.orig
+++ contrib/mod_tls.c
@@ -8070,10 +8070,13 @@ static int tls_writemore(int wfd) {
 
 static ssize_t tls_read(SSL *ssl, void *buf, size_t len) {
   ssize_t count;
+  int xerrno = 0;
 
   retry:
   pr_signals_handle();
   count = SSL_read(ssl, buf, len);
+  xerrno = errno;
+
   if (count < 0) {
     long err;
     int fd;
@@ -8099,7 +8102,7 @@ static ssize_t tls_read(SSL *ssl, void *buf, size_t len) {
         } else if (err == 0) {
           /* Still missing data after timeout. Simulate an EINTR and return.
            */
-          errno = EINTR;
+          xerrno = EINTR;
 
           /* If err < 0, i.e. some error from the select(), everything is
            * already in place; errno is properly set and this function
@@ -8122,7 +8125,7 @@ static ssize_t tls_read(SSL *ssl, void *buf, size_t len) {
         } else if (err == 0) {
           /* Still missing data after timeout. Simulate an EINTR and return.
            */
-          errno = EINTR;
+          xerrno = EINTR;
 
           /* If err < 0, i.e. some error from the select(), everything is
            * already in place; errno is properly set and this function
@@ -8141,6 +8144,7 @@ static ssize_t tls_read(SSL *ssl, void *buf, size_t len) {
     }
   }
 
+  errno = xerrno;
   return count;
 }
 
@@ -9479,8 +9483,11 @@ static int tls_verify_ocsp(int ok, X509_STORE_CTX *ctx) {
 
 static ssize_t tls_write(SSL *ssl, const void *buf, size_t len) {
   ssize_t count;
+  int xerrno = 0;
 
   count = SSL_write(ssl, buf, len);
+  xerrno = errno;
+
   if (count < 0) {
     long err = SSL_get_error(ssl, count);
 
@@ -9491,7 +9498,7 @@ static ssize_t tls_write(SSL *ssl, const void *buf, size_t len) {
       case SSL_ERROR_WANT_READ:
       case SSL_ERROR_WANT_WRITE:
         /* Simulate an EINTR in case OpenSSL wants to write more. */
-        errno = EINTR;
+        xerrno = EINTR;
         break;
 
       default:
@@ -9529,6 +9536,7 @@ static ssize_t tls_write(SSL *ssl, const void *buf, size_t len) {
     tls_data_adaptive_bytes_written_ms = now;
   }
 
+  errno = xerrno;
   return count;
 }
 
@@ -10718,7 +10726,7 @@ static int tls_netio_read_cb(pr_netio_stream_t *nstrm, char *buf,
   ssl = (SSL *) pr_table_get(nstrm->notes, TLS_NETIO_NOTE, NULL);
   if (ssl != NULL) {
     BIO *rbio, *wbio;
-    int bread = 0, bwritten = 0;
+    int bread = 0, bwritten = 0, xerrno = 0;
     ssize_t res = 0;
     unsigned long rbio_rbytes, rbio_wbytes, wbio_rbytes, wbio_wbytes;
 
@@ -10731,6 +10739,7 @@ static int tls_netio_read_cb(pr_netio_stream_t *nstrm, char *buf,
     wbio_wbytes = BIO_number_written(wbio);
 
     res = tls_read(ssl, buf, buflen);
+    xerrno = errno;
 
     bread = (BIO_number_read(rbio) - rbio_rbytes) +
       (BIO_number_read(wbio) - wbio_rbytes);
@@ -10752,6 +10761,7 @@ static int tls_netio_read_cb(pr_netio_stream_t *nstrm, char *buf,
       session.total_raw_out += bwritten;
     }
 
+    errno = xerrno;
     return res;
   }
 
@@ -10858,7 +10868,7 @@ static int tls_netio_write_cb(pr_netio_stream_t *nstrm, char *buf,
   ssl = (SSL *) pr_table_get(nstrm->notes, TLS_NETIO_NOTE, NULL);
   if (ssl != NULL) {
     BIO *rbio, *wbio;
-    int bread = 0, bwritten = 0;
+    int bread = 0, bwritten = 0, xerrno = 0;
     ssize_t res = 0;
     unsigned long rbio_rbytes, rbio_wbytes, wbio_rbytes, wbio_wbytes;
 
@@ -10899,6 +10909,7 @@ static int tls_netio_write_cb(pr_netio_stream_t *nstrm, char *buf,
 #endif
 
     res = tls_write(ssl, buf, buflen);
+    xerrno = errno;
 
     bread = (BIO_number_read(rbio) - rbio_rbytes) +
       (BIO_number_read(wbio) - wbio_rbytes);
@@ -10920,6 +10931,7 @@ static int tls_netio_write_cb(pr_netio_stream_t *nstrm, char *buf,
       session.total_raw_out += (bwritten - res);
     }
 
+    errno = xerrno;
     return res;
   }
 
