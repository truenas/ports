--- src/providers/ldap/sdap_access.c.orig	2018-10-01 06:55:21.804740000 -0400
+++ src/providers/ldap/sdap_access.c	2018-10-01 06:55:54.858118000 -0400
@@ -557,9 +557,9 @@ bool nds_check_expired(const char *exp_t
 
     now = time(NULL);
     DEBUG(SSSDBG_TRACE_ALL,
-          "Time info: tzname[0] [%s] tzname[1] [%s] timezone [%ld] "
-           "daylight [%d] now [%ld] expire_time [%ld].\n", tzname[0],
-           tzname[1], timezone, daylight, now, expire_time);
+          "Time info: tzname[0] [%s] tzname[1] [%s]"
+           "now [%ld] expire_time [%ld].\n", tzname[0],
+           tzname[1], now, expire_time);
 
     if (difftime(now, expire_time) > 0.0) {
         DEBUG(SSSDBG_CONF_SETTINGS, "NDS account expired.\n");
