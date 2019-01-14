--- src/tools/hunspell.cxx.orig	2019-01-14 10:46:40 UTC
+++ src/tools/hunspell.cxx
@@ -116,28 +116,22 @@
 #include "../parsers/odfparser.hxx"
 
 #define LIBDIR                \
-  "/usr/share/hunspell:"      \
-  "/usr/share/myspell:"       \
-  "/usr/share/myspell/dicts:" \
+  "%%PREFIX%%/share/hunspell:"      \
+  "%%PREFIX%%/share/myspell:"       \
+  "%%PREFIX%%/share/myspell/dicts:" \
   "/Library/Spelling"
 #define USEROOODIR {                  \
-  ".openoffice.org/3/user/wordbook", \
-  ".openoffice.org2/user/wordbook",  \
-  ".openoffice.org2.0/user/wordbook",\
+  ".openoffice.org/3/user/wordbook" \
+  ".openoffice.org2/user/wordbook"  \
+  ".openoffice.org2.0/user/wordbook"\
   "Library/Spelling" }
 #define OOODIR                                       \
-  "/opt/openoffice.org/basis3.0/share/dict/ooo:"     \
-  "/usr/lib/openoffice.org/basis3.0/share/dict/ooo:" \
-  "/opt/openoffice.org2.4/share/dict/ooo:"           \
-  "/usr/lib/openoffice.org2.4/share/dict/ooo:"       \
-  "/opt/openoffice.org2.3/share/dict/ooo:"           \
-  "/usr/lib/openoffice.org2.3/share/dict/ooo:"       \
-  "/opt/openoffice.org2.2/share/dict/ooo:"           \
-  "/usr/lib/openoffice.org2.2/share/dict/ooo:"       \
-  "/opt/openoffice.org2.1/share/dict/ooo:"           \
-  "/usr/lib/openoffice.org2.1/share/dict/ooo:"       \
-  "/opt/openoffice.org2.0/share/dict/ooo:"           \
-  "/usr/lib/openoffice.org2.0/share/dict/ooo"
+  "%%PREFIX%%/openoffice.org/basis3.0/share/dict/ooo:"     \
+  "%%PREFIX%%/openoffice.org2.4/share/dict/ooo:"           \
+  "%%PREFIX%%/openoffice.org2.3/share/dict/ooo:"           \
+  "%%PREFIX%%/openoffice.org2.2/share/dict/ooo:"           \
+  "%%PREFIX%%/openoffice.org2.1/share/dict/ooo:"           \
+  "%%PREFIX%%/openoffice.org2.0/share/dict/ooo:"
 #define HOME getenv("HOME")
 #define DICBASENAME ".hunspell_"
 #define LOGFILE "/tmp/hunspell.log"
