--- lib/util/util.c.orig	2019-04-05 13:23:32.418930307 -0700
+++ lib/util/util.c	2019-04-05 13:24:43.961904372 -0700
@@ -216,7 +216,7 @@ _PUBLIC_ bool directory_create_or_exist(
 			return false;
 		}
 
-		if (!S_ISDIR(sbuf.st_mode)) {
+		if (!S_ISDIR(sbuf.st_mode) && !S_ISLNK(sbuf.st_mode)) {
 			return false;
 		}
 	}
