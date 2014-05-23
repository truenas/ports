--- ./libatalk/vfs/extattr.c
+++ /tmp/extattr.c
@@ -193,6 +193,8 @@ ssize_t sys_fgetxattr (int filedes, const char *uname, void *value, size_t size)
     const char *attrname = ((s=strchr(name, '.')) == NULL) ? name : s + 1;
 
     if((retval=extattr_get_fd(filedes, attrnamespace, attrname, NULL, 0)) >= 0) {
+	if (size == 0)
+		return retval;
         if(retval > size) {
             errno = ERANGE;
             return -1;
@@ -241,6 +243,8 @@ ssize_t sys_lgetxattr (const char *path, const char *uname, void *value, size_t
 #elif defined(HAVE_EXTATTR_GET_LINK)
 	ssize_t retval;
 	if((retval=extattr_get_link(path, EXTATTR_NAMESPACE_USER, uname, NULL, 0)) >= 0) {
+		if (size == 0)
+			return retval;
 		if(retval > size) {
 			errno = ERANGE;
 			return -1;
