--- src/_cffi_src/openssl/callbacks.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/callbacks.py
@@ -11,7 +11,10 @@ INCLUDES = """
 #include <openssl/crypto.h>
 
 #ifdef _WIN32
+#define WIN32_LEAN_AND_MEAN
 #include <Windows.h>
+#include <Wincrypt.h>
+#include <Winsock2.h>
 #else
 #include <stdio.h>
 #include <stdlib.h>
