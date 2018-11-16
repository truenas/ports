--- src/util/util.c.orig	2018-10-01 06:06:56.462785000 -0400
+++ src/util/util.c	2018-10-01 06:09:27.040843000 -0400
@@ -967,6 +967,7 @@ errno_t sss_utc_to_time_t(const char *st
         return EINVAL;
     }
 
+    tzset();
     ut = mktime(&tm);
     if (ut == -1) {
         DEBUG(SSSDBG_TRACE_INTERNAL,
@@ -974,8 +975,6 @@ errno_t sss_utc_to_time_t(const char *st
         return EINVAL;
     }
 
-    tzset();
-    ut -= timezone;
     *_unix_time = ut;
     return EOK;
 }
