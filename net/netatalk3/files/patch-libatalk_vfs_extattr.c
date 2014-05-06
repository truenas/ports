--- ./libatalk/vfs/extattr.c	2013-10-18 04:57:33.000000000 -0700
+++ /tmp/extattr.c	2014-05-06 11:31:34.000000000 -0700
@@ -241,6 +241,10 @@
 #elif defined(HAVE_EXTATTR_GET_LINK)
 	ssize_t retval;
 	if((retval=extattr_get_link(path, EXTATTR_NAMESPACE_USER, uname, NULL, 0)) >= 0) {
+		if (size == 0) {
+			/* size == 0 means only return size */
+			return retval;
+		}
 		if(retval > size) {
 			errno = ERANGE;
 			return -1;
