--- src/_cffi_src/openssl/err.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/err.py
@@ -12,13 +12,6 @@ TYPES = """
 static const int Cryptography_HAS_EC_CODES;
 static const int Cryptography_HAS_RSA_R_PKCS_DECODING_ERROR;
 
-struct ERR_string_data_st {
-    unsigned long error;
-    const char *string;
-};
-typedef struct ERR_string_data_st ERR_STRING_DATA;
-typedef ... ERR_STATE;
-
 static const int ERR_LIB_DH;
 static const int ERR_LIB_EVP;
 static const int ERR_LIB_EC;
@@ -29,36 +22,6 @@ static const int ERR_LIB_PKCS12;
 static const int ERR_LIB_SSL;
 static const int ERR_LIB_X509;
 
-static const int ASN1_F_ASN1_EX_C2I;
-static const int ASN1_F_ASN1_FIND_END;
-static const int ASN1_F_ASN1_GENERATE_V3;
-static const int ASN1_F_ASN1_GET_OBJECT;
-static const int ASN1_F_ASN1_ITEM_I2D_FP;
-static const int ASN1_F_ASN1_ITEM_PACK;
-static const int ASN1_F_ASN1_ITEM_SIGN;
-static const int ASN1_F_ASN1_ITEM_UNPACK;
-static const int ASN1_F_ASN1_ITEM_VERIFY;
-static const int ASN1_F_ASN1_MBSTRING_NCOPY;
-static const int ASN1_F_ASN1_TEMPLATE_EX_D2I;
-static const int ASN1_F_ASN1_TEMPLATE_NEW;
-static const int ASN1_F_ASN1_TEMPLATE_NOEXP_D2I;
-static const int ASN1_F_ASN1_TYPE_GET_INT_OCTETSTRING;
-static const int ASN1_F_ASN1_TYPE_GET_OCTETSTRING;
-static const int ASN1_F_ASN1_VERIFY;
-static const int ASN1_F_BITSTR_CB;
-static const int ASN1_F_D2I_ASN1_UINTEGER;
-static const int ASN1_F_D2I_PRIVATEKEY;
-static const int ASN1_F_I2D_DSA_PUBKEY;
-static const int ASN1_F_LONG_C2I;
-static const int ASN1_F_OID_MODULE_INIT;
-static const int ASN1_F_PARSE_TAGGING;
-static const int ASN1_F_PKCS5_PBE_SET;
-static const int ASN1_F_B64_READ_ASN1;
-static const int ASN1_F_B64_WRITE_ASN1;
-static const int ASN1_F_SMIME_READ_ASN1;
-static const int ASN1_F_SMIME_TEXT;
-static const int ASN1_F_ASN1_CHECK_TLEN;
-
 static const int ASN1_R_BOOLEAN_IS_WRONG_LENGTH;
 static const int ASN1_R_BUFFER_TOO_SMALL;
 static const int ASN1_R_CIPHER_HAS_NO_OBJECT_IDENTIFIER;
@@ -84,33 +47,9 @@ static const int ASN1_R_NO_MULTIPART_BODY_FAILURE;
 static const int ASN1_R_NO_MULTIPART_BOUNDARY;
 static const int ASN1_R_HEADER_TOO_LONG;
 
-static const int DH_F_COMPUTE_KEY;
-
 static const int DH_R_INVALID_PUBKEY;
 
-static const int EVP_F_AES_INIT_KEY;
-static const int EVP_F_EVP_CIPHER_CTX_CTRL;
-static const int EVP_F_EVP_CIPHER_CTX_SET_KEY_LENGTH;
-static const int EVP_F_EVP_CIPHERINIT_EX;
-static const int EVP_F_EVP_DECRYPTFINAL_EX;
-static const int EVP_F_EVP_DIGESTINIT_EX;
 static const int EVP_F_EVP_ENCRYPTFINAL_EX;
-static const int EVP_F_EVP_MD_CTX_COPY_EX;
-static const int EVP_F_EVP_OPENINIT;
-static const int EVP_F_EVP_PBE_ALG_ADD;
-static const int EVP_F_EVP_PBE_CIPHERINIT;
-static const int EVP_F_EVP_PKCS82PKEY;
-static const int EVP_F_EVP_PKEY_COPY_PARAMETERS;
-static const int EVP_F_EVP_PKEY_DECRYPT;
-static const int EVP_F_EVP_PKEY_ENCRYPT;
-static const int EVP_F_EVP_PKEY_NEW;
-static const int EVP_F_EVP_SIGNFINAL;
-static const int EVP_F_EVP_VERIFYFINAL;
-static const int EVP_F_PKCS5_PBE_KEYIVGEN;
-static const int EVP_F_PKCS5_V2_PBE_KEYIVGEN;
-static const int EVP_F_RC2_MAGIC_TO_METH;
-static const int EVP_F_RC5_CTRL;
-static const int EVP_F_CAMELLIA_INIT_KEY;
 
 static const int EVP_R_AES_KEY_SETUP_FAILED;
 static const int EVP_R_BAD_DECRYPT;
@@ -137,33 +76,8 @@ static const int EVP_R_UNSUPPORTED_PRIVATE_KEY_ALGORIT
 static const int EVP_R_WRONG_FINAL_BLOCK_LENGTH;
 static const int EVP_R_CAMELLIA_KEY_SETUP_FAILED;
 
-static const int EC_F_EC_GROUP_NEW_BY_CURVE_NAME;
-
 static const int EC_R_UNKNOWN_GROUP;
 
-static const int PEM_F_D2I_PKCS8PRIVATEKEY_BIO;
-static const int PEM_F_D2I_PKCS8PRIVATEKEY_FP;
-static const int PEM_F_DO_PK8PKEY;
-static const int PEM_F_DO_PK8PKEY_FP;
-static const int PEM_F_LOAD_IV;
-static const int PEM_F_PEM_ASN1_READ;
-static const int PEM_F_PEM_ASN1_READ_BIO;
-static const int PEM_F_PEM_ASN1_WRITE;
-static const int PEM_F_PEM_ASN1_WRITE_BIO;
-static const int PEM_F_PEM_DEF_CALLBACK;
-static const int PEM_F_PEM_DO_HEADER;
-static const int PEM_F_PEM_GET_EVP_CIPHER_INFO;
-static const int PEM_F_PEM_READ;
-static const int PEM_F_PEM_READ_BIO;
-static const int PEM_F_PEM_READ_BIO_PRIVATEKEY;
-static const int PEM_F_PEM_READ_PRIVATEKEY;
-static const int PEM_F_PEM_SIGNFINAL;
-static const int PEM_F_PEM_WRITE;
-static const int PEM_F_PEM_WRITE_BIO;
-static const int PEM_F_PEM_X509_INFO_READ;
-static const int PEM_F_PEM_X509_INFO_READ_BIO;
-static const int PEM_F_PEM_X509_INFO_WRITE_BIO;
-
 static const int PEM_R_BAD_BASE64_DECODE;
 static const int PEM_R_BAD_DECRYPT;
 static const int PEM_R_BAD_END_LINE;
@@ -180,8 +94,6 @@ static const int PEM_R_SHORT_HEADER;
 static const int PEM_R_UNSUPPORTED_CIPHER;
 static const int PEM_R_UNSUPPORTED_ENCRYPTION;
 
-static const int PKCS12_F_PKCS12_PBE_CRYPT;
-
 static const int PKCS12_R_PKCS12_CIPHERFINAL_ERROR;
 
 static const int RSA_R_DATA_TOO_LARGE_FOR_KEY_SIZE;
@@ -191,7 +103,6 @@ static const int RSA_R_BLOCK_TYPE_IS_NOT_01;
 static const int RSA_R_BLOCK_TYPE_IS_NOT_02;
 static const int RSA_R_PKCS_DECODING_ERROR;
 static const int RSA_R_OAEP_DECODING_ERROR;
-static const int RSA_F_RSA_SIGN;
 
 static const int SSL_TLSEXT_ERR_OK;
 static const int SSL_TLSEXT_ERR_ALERT_WARNING;
@@ -235,29 +146,12 @@ void ERR_error_string_n(unsigned long, char *, size_t)
 const char *ERR_lib_error_string(unsigned long);
 const char *ERR_func_error_string(unsigned long);
 const char *ERR_reason_error_string(unsigned long);
-void ERR_print_errors(BIO *);
-void ERR_print_errors_fp(FILE *);
 unsigned long ERR_get_error(void);
 unsigned long ERR_peek_error(void);
 unsigned long ERR_peek_last_error(void);
-unsigned long ERR_get_error_line(const char **, int *);
-unsigned long ERR_peek_error_line(const char **, int *);
-unsigned long ERR_peek_last_error_line(const char **, int *);
-unsigned long ERR_get_error_line_data(const char **, int *,
-                                      const char **, int *);
 void ERR_clear_error(void);
-unsigned long ERR_peek_error_line_data(const char **,
-                                       int *, const char **, int *);
-unsigned long ERR_peek_last_error_line_data(const char **,
-                                            int *, const char **, int *);
 void ERR_put_error(int, int, int, const char *, int);
-void ERR_add_error_data(int, ...);
-int ERR_get_next_error_library(void);
-ERR_STATE *ERR_get_state(void);
-/* ERR_free_strings became a macro in 1.1.0 */
-void ERR_free_strings(void);
 
-unsigned long ERR_PACK(int, int, int);
 int ERR_GET_LIB(unsigned long);
 int ERR_GET_FUNC(unsigned long);
 int ERR_GET_REASON(unsigned long);
