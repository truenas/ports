--- version-gen.sh.orig	2019-06-13 09:32:35 UTC
+++ version-gen.sh
@@ -2,10 +2,6 @@
 
 DEFAULT_VERSION="5.11.0.git"
 
-if [ -d .git ]; then
-	VERSION="`git describe --dirty=+ --abbrev=7 2> /dev/null | sed -e '/^collectd-/!d' -e 's///' -e 'y/-/./'`"
-fi
-
 if test -z "$VERSION"; then
 	VERSION="$DEFAULT_VERSION"
 fi
