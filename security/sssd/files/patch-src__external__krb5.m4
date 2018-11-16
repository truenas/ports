--- src/external/krb5.m4.orig	2016-04-13 10:48:41.000000000 -0400
+++ src/external/krb5.m4	2018-10-01 05:14:12.770416000 -0400
@@ -9,7 +9,7 @@ if test x$KRB5_CFLAGS != x; then
     KRB5_PASSED_CFLAGS=$KRB5_CFLAGS
 fi
 
-AC_PATH_PROG(KRB5_CONFIG, krb5-config)
+AC_PATH_PROG(KRB5_CONFIG, krb5-config, [], [/usr/local/bin:$PATH])
 AC_MSG_CHECKING(for working krb5-config)
 if test -x "$KRB5_CONFIG"; then
   KRB5_CFLAGS="`$KRB5_CONFIG --cflags`"
