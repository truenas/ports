--- src/external/pac_responder.m4.orig	2016-04-13 10:48:41.000000000 -0400
+++ src/external/pac_responder.m4	2018-10-01 05:42:40.730913000 -0400
@@ -15,6 +15,7 @@ then
         AC_MSG_WARN([Cannot build pac responder without libndr_krb5pac]))
 
     AC_PATH_PROG(KRB5_CONFIG, krb5-config)
+    AC_PATH_PROG(KRB5_CONFIG, krb5-config, [], [/usr/local/bin:$PATH])
     AC_MSG_CHECKING(for supported MIT krb5 version)
     KRB5_VERSION="`$KRB5_CONFIG --version`"
     case $KRB5_VERSION in
@@ -23,7 +24,9 @@ then
         Kerberos\ 5\ release\ 1.11* | \
         Kerberos\ 5\ release\ 1.12* | \
         Kerberos\ 5\ release\ 1.13* | \
-        Kerberos\ 5\ release\ 1.14*)
+        Kerberos\ 5\ release\ 1.14* | \
+        Kerberos\ 5\ release\ 1.15* | \
+        Kerberos\ 5\ release\ 1.16*)
             krb5_version_ok=yes
             AC_MSG_RESULT([yes])
             ;;
