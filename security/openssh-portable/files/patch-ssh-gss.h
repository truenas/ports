--- ssh-gss.h.orig
+++ ssh-gss.h
@@ -28,11 +28,7 @@
 
 #ifdef GSSAPI
 
-#ifdef HAVE_GSSAPI_H
-#include <gssapi.h>
-#elif defined(HAVE_GSSAPI_GSSAPI_H)
 #include <gssapi/gssapi.h>
-#endif
 
 #ifdef KRB5
 # ifndef HEIMDAL
