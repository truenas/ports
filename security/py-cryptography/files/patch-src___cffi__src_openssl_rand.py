--- src/_cffi_src/openssl/rand.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/rand.py
@@ -13,12 +13,8 @@ static const long Cryptography_HAS_EGD;
 """
 
 FUNCTIONS = """
-void RAND_seed(const void *, int);
 void RAND_add(const void *, int, double);
 int RAND_status(void);
-const char *RAND_file_name(char *, size_t);
-int RAND_load_file(const char *, long);
-int RAND_write_file(const char *);
 int RAND_bytes(unsigned char *, int);
 /* ERR_load_RAND_strings started returning an int in 1.1.0. Unfortunately we
    can't declare a conditional signature like that. Since it always returns
