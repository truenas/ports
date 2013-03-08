--- /dev/null	2011-02-22 08:51:14.000000000 -0800
+++ ./lib/sysacls.c	2011-10-20 23:11:48.000000000 -0700
@@ -80,12 +80,36 @@
 	return acl_get_file( path_p, type);
 }
 
-#if 0
 SMB_ACL_T sys_acl_get_fd(int fd)
 {
 	return acl_get_fd(fd);
 }
-#endif
+
+int sys_acl_get_brand( SMB_ACL_T the_acl, int *brand_p)
+{
+	return acl_get_brand_np(the_acl, brand_p);
+}
+
+int sys_acl_get_brand_file( const char *path_p, int *brand_p)
+{
+	int fd;
+	acl_t acl;
+
+	if ((fd = open(path_p, O_RDONLY|O_NONBLOCK)) < 0)
+		return -1;
+	if ((acl = acl_get_fd(fd)) == NULL) {
+		close(fd);
+		return -1;
+	}
+	close(fd);
+	if (acl_get_brand_np(acl, brand_p) < 0) {
+		acl_free(acl);
+		return -1;
+	}
+
+	acl_free(acl);
+	return 0;
+}
 
 #if defined(HAVE_ACL_GET_PERM_NP)
 #define acl_get_perm(p, b) acl_get_perm_np(p, b)
