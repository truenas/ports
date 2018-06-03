--- src/_cffi_src/openssl/crypto.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/crypto.py
@@ -143,6 +143,6 @@ void *Cryptography_realloc_wrapper(void *ptr, size_t s
 }
 
 void Cryptography_free_wrapper(void *ptr, const char *path, int line) {
-    return free(ptr);
+    free(ptr);
 }
 """
