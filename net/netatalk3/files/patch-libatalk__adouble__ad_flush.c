--- libatalk/adouble/ad_flush.c.orig	2022-03-31 14:51:49.834708000 -0400
+++ libatalk/adouble/ad_flush.c	2022-03-31 14:52:07.993604000 -0400
@@ -39,6 +39,7 @@
 #include <atalk/bstradd.h>
 #include <atalk/errchk.h>
 #include <atalk/util.h>
+#include <atalk/unix.h>
 
 #include "ad_lock.h"
 
