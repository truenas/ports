--- src/util.c.orig	2011-12-20 14:11:03.000000000 +0100
+++ src/util.c	2015-03-08 22:30:38.673935000 +0100
@@ -167,7 +167,7 @@
 
 	if (stat(component, &st) != 0)
 	    my_fatale("stat(\"%s\")", component);
-	if (st.st_uid != 0 || (st.st_mode & 022) != 0)
+	if (st.st_uid != 0)
 	    my_fatal("bad ownership or modes for chroot "
 		    "directory %s\"%s\"",
 		    cp == NULL ? "" : "component ", component);
@@ -213,9 +213,6 @@
     FILE *file;
     int ret = 0;
 
-    if (path == NULL || line == NULL)
-	return(0);
-
     if ((file = fopen(path, "r")) == NULL)
 	return(0);
 
@@ -232,9 +229,6 @@
 int write_line(const char *path, char *line, uint16_t len) {
     int fd, ret;
 
-    if (path == NULL || line == NULL)
-	return(0);
-
     if ((fd = open(path, O_WRONLY|O_TRUNC)) == -1)
 	return(0);
 
