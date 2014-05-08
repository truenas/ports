--- ./src/util.c.orig	2014-05-08 06:11:36.536981238 -0700
+++ ./src/util.c	2014-05-08 06:12:10.816170323 -0700
@@ -167,7 +167,7 @@
 
 	if (stat(component, &st) != 0)
 	    my_fatale("stat(\"%s\")", component);
-	if (st.st_uid != 0 || (st.st_mode & 022) != 0)
+	if (st.st_uid != 0)
 	    my_fatal("bad ownership or modes for chroot "
 		    "directory %s\"%s\"",
 		    cp == NULL ? "" : "component ", component);
