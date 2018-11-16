--- src/util/cert/nss/cert.c.orig	2018-10-01 06:16:04.509852000 -0400
+++ src/util/cert/nss/cert.c	2018-10-01 06:18:27.476961000 -0400
@@ -26,6 +26,7 @@
 #include <key.h>
 #include <prerror.h>
 #include <ocsp.h>
+#include "src/util/byte-swapping.h"
 #include <talloc.h>
 
 #include "util/crypto/sss_crypto.h"
