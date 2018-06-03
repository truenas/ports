--- src/_cffi_src/openssl/ec.py.orig	2018-03-27 14:12:05 UTC
+++ src/_cffi_src/openssl/ec.py
@@ -27,34 +27,16 @@ typedef struct {
 typedef enum {
     POINT_CONVERSION_COMPRESSED,
     POINT_CONVERSION_UNCOMPRESSED,
-    POINT_CONVERSION_HYBRID,
     ...
 } point_conversion_form_t;
 """
 
 FUNCTIONS = """
-EC_GROUP *EC_GROUP_new(const EC_METHOD *);
 void EC_GROUP_free(EC_GROUP *);
-void EC_GROUP_clear_free(EC_GROUP *);
 
-EC_GROUP *EC_GROUP_new_curve_GFp(
-    const BIGNUM *, const BIGNUM *, const BIGNUM *, BN_CTX *);
-EC_GROUP *EC_GROUP_new_curve_GF2m(
-    const BIGNUM *, const BIGNUM *, const BIGNUM *, BN_CTX *);
 EC_GROUP *EC_GROUP_new_by_curve_name(int);
 
-int EC_GROUP_set_curve_GFp(
-    EC_GROUP *, const BIGNUM *, const BIGNUM *, const BIGNUM *, BN_CTX *);
-int EC_GROUP_get_curve_GFp(
-    const EC_GROUP *, BIGNUM *, BIGNUM *, BIGNUM *, BN_CTX *);
-int EC_GROUP_set_curve_GF2m(
-    EC_GROUP *, const BIGNUM *, const BIGNUM *, const BIGNUM *, BN_CTX *);
-int EC_GROUP_get_curve_GF2m(
-    const EC_GROUP *, BIGNUM *, BIGNUM *, BIGNUM *, BN_CTX *);
-
 int EC_GROUP_get_degree(const EC_GROUP *);
-void EC_GROUP_set_asn1_flag(EC_GROUP *, int);
-void EC_GROUP_set_point_conversion_form(EC_GROUP *, point_conversion_form_t);
 
 const EC_METHOD *EC_GROUP_method_of(const EC_GROUP *);
 const EC_POINT *EC_GROUP_get0_generator(const EC_GROUP *);
@@ -65,13 +47,7 @@ size_t EC_get_builtin_curves(EC_builtin_curve *, size_
 EC_KEY *EC_KEY_new(void);
 void EC_KEY_free(EC_KEY *);
 
-int EC_KEY_get_flags(const EC_KEY *);
-void EC_KEY_set_flags(EC_KEY *, int);
-void EC_KEY_clear_flags(EC_KEY *, int);
 EC_KEY *EC_KEY_new_by_curve_name(int);
-EC_KEY *EC_KEY_copy(EC_KEY *, EC_KEY *);
-EC_KEY *EC_KEY_dup(EC_KEY *);
-int EC_KEY_up_ref(EC_KEY *);
 const EC_GROUP *EC_KEY_get0_group(const EC_KEY *);
 int EC_GROUP_get_order(const EC_GROUP *, BIGNUM *, BN_CTX *);
 int EC_KEY_set_group(EC_KEY *, const EC_GROUP *);
@@ -79,31 +55,15 @@ const BIGNUM *EC_KEY_get0_private_key(const EC_KEY *);
 int EC_KEY_set_private_key(EC_KEY *, const BIGNUM *);
 const EC_POINT *EC_KEY_get0_public_key(const EC_KEY *);
 int EC_KEY_set_public_key(EC_KEY *, const EC_POINT *);
-unsigned int EC_KEY_get_enc_flags(const EC_KEY *);
-void EC_KEY_set_enc_flags(EC_KEY *eckey, unsigned int);
-point_conversion_form_t EC_KEY_get_conv_form(const EC_KEY *);
-void EC_KEY_set_conv_form(EC_KEY *, point_conversion_form_t);
 void EC_KEY_set_asn1_flag(EC_KEY *, int);
-int EC_KEY_precompute_mult(EC_KEY *, BN_CTX *);
 int EC_KEY_generate_key(EC_KEY *);
-int EC_KEY_check_key(const EC_KEY *);
 int EC_KEY_set_public_key_affine_coordinates(EC_KEY *, BIGNUM *, BIGNUM *);
 
 EC_POINT *EC_POINT_new(const EC_GROUP *);
 void EC_POINT_free(EC_POINT *);
 void EC_POINT_clear_free(EC_POINT *);
-int EC_POINT_copy(EC_POINT *, const EC_POINT *);
 EC_POINT *EC_POINT_dup(const EC_POINT *, const EC_GROUP *);
-const EC_METHOD *EC_POINT_method_of(const EC_POINT *);
 
-int EC_POINT_set_to_infinity(const EC_GROUP *, EC_POINT *);
-
-int EC_POINT_set_Jprojective_coordinates_GFp(const EC_GROUP *, EC_POINT *,
-    const BIGNUM *, const BIGNUM *, const BIGNUM *, BN_CTX *);
-
-int EC_POINT_get_Jprojective_coordinates_GFp(const EC_GROUP *,
-    const EC_POINT *, BIGNUM *, BIGNUM *, BIGNUM *, BN_CTX *);
-
 int EC_POINT_set_affine_coordinates_GFp(const EC_GROUP *, EC_POINT *,
     const BIGNUM *, const BIGNUM *, BN_CTX *);
 
@@ -129,18 +89,6 @@ size_t EC_POINT_point2oct(const EC_GROUP *, const EC_P
 int EC_POINT_oct2point(const EC_GROUP *, EC_POINT *,
     const unsigned char *, size_t, BN_CTX *);
 
-BIGNUM *EC_POINT_point2bn(const EC_GROUP *, const EC_POINT *,
-    point_conversion_form_t form, BIGNUM *, BN_CTX *);
-
-EC_POINT *EC_POINT_bn2point(const EC_GROUP *, const BIGNUM *,
-    EC_POINT *, BN_CTX *);
-
-char *EC_POINT_point2hex(const EC_GROUP *, const EC_POINT *,
-    point_conversion_form_t form, BN_CTX *);
-
-EC_POINT *EC_POINT_hex2point(const EC_GROUP *, const char *,
-    EC_POINT *, BN_CTX *);
-
 int EC_POINT_add(const EC_GROUP *, EC_POINT *, const EC_POINT *,
     const EC_POINT *, BN_CTX *);
 
@@ -152,25 +100,9 @@ int EC_POINT_is_on_curve(const EC_GROUP *, const EC_PO
 int EC_POINT_cmp(
     const EC_GROUP *, const EC_POINT *, const EC_POINT *, BN_CTX *);
 
-int EC_POINT_make_affine(const EC_GROUP *, EC_POINT *, BN_CTX *);
-int EC_POINTs_make_affine(const EC_GROUP *, size_t, EC_POINT *[], BN_CTX *);
-
-int EC_POINTs_mul(
-    const EC_GROUP *, EC_POINT *, const BIGNUM *,
-    size_t, const EC_POINT *[], const BIGNUM *[], BN_CTX *);
-
 int EC_POINT_mul(const EC_GROUP *, EC_POINT *, const BIGNUM *,
     const EC_POINT *, const BIGNUM *, BN_CTX *);
 
-int EC_GROUP_precompute_mult(EC_GROUP *, BN_CTX *);
-int EC_GROUP_have_precompute_mult(const EC_GROUP *);
-
-const EC_METHOD *EC_GFp_simple_method();
-const EC_METHOD *EC_GFp_mont_method();
-const EC_METHOD *EC_GFp_nist_method();
-
-const EC_METHOD *EC_GF2m_simple_method();
-
 int EC_METHOD_get_field_type(const EC_METHOD *);
 
 const char *EC_curve_nid2nist(int);
@@ -182,8 +114,6 @@ static const long Cryptography_HAS_EC = 1;
 #if defined(OPENSSL_NO_EC2M)
 static const long Cryptography_HAS_EC2M = 0;
 
-const EC_METHOD *(*EC_GF2m_simple_method)() = NULL;
-
 int (*EC_POINT_set_affine_coordinates_GF2m)(const EC_GROUP *, EC_POINT *,
     const BIGNUM *, const BIGNUM *, BN_CTX *) = NULL;
 
@@ -192,15 +122,6 @@ int (*EC_POINT_get_affine_coordinates_GF2m)(const EC_G
 
 int (*EC_POINT_set_compressed_coordinates_GF2m)(const EC_GROUP *, EC_POINT *,
     const BIGNUM *, int, BN_CTX *) = NULL;
-
-int (*EC_GROUP_set_curve_GF2m)(
-    EC_GROUP *, const BIGNUM *, const BIGNUM *, const BIGNUM *, BN_CTX *);
-
-int (*EC_GROUP_get_curve_GF2m)(
-    const EC_GROUP *, BIGNUM *, BIGNUM *, BIGNUM *, BN_CTX *);
-
-EC_GROUP *(*EC_GROUP_new_curve_GF2m)(
-    const BIGNUM *, const BIGNUM *, const BIGNUM *, BN_CTX *);
 #else
 static const long Cryptography_HAS_EC2M = 1;
 #endif
