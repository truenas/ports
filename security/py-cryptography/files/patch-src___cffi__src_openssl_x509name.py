--- src/_cffi_src/openssl/x509name.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/x509name.py
@@ -40,6 +40,7 @@ int Cryptography_X509_NAME_ENTRY_set(X509_NAME_ENTRY *
 int X509_NAME_entry_count(X509_NAME *);
 X509_NAME_ENTRY *X509_NAME_get_entry(X509_NAME *, int);
 char *X509_NAME_oneline(X509_NAME *, char *, int);
+int X509_NAME_print_ex(BIO *, X509_NAME *, int, unsigned long);
 
 /* These became const X509_NAME_ENTRY * in 1.1.0 */
 ASN1_OBJECT *X509_NAME_ENTRY_get_object(X509_NAME_ENTRY *);
