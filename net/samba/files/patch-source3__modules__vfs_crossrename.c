--- source3/modules/vfs_crossrename.c.orig	2020-01-31 05:25:36.000000000 -0500
+++ source3/modules/vfs_crossrename.c	2021-07-07 05:18:11.564346000 -0400
@@ -24,22 +24,65 @@
 #define MODULE "crossrename"
 static off_t module_sizelimit;
 
+static bool conv_str_off_t_error(const char *str, off_t *valp)
+{
+	char *end = NULL;
+	long lval;
+
+	lval = strtol(str, &end, 10);
+	if (end == NULL || *end != '\0' || end == str) {
+		DBG_ERR("%s: Failed to convert to size. "
+			"sizelimit must be expressed as "
+			"integer without a suffix.\n", str);
+		return false;
+	}
+	if ((lval == LONG_MIN || lval == LONG_MAX) &&
+	    errno == ERANGE) {
+		DBG_ERR("%s: invalid size for sizelimit.",
+			str);
+		return false;
+	}
+
+	lval *= 1048576;
+
+	*valp = (off_t)lval;
+	return true;
+}
+
 static int crossrename_connect(
                 struct vfs_handle_struct *  handle,
                 const char *                service,
                 const char *                user)
 {
+	const char *module_sizelimit_str = NULL;
+	bool ok;
+
 	int ret = SMB_VFS_NEXT_CONNECT(handle, service, user);
 
 	if (ret < 0) {
 		return ret;
 	}
 
+	module_sizelimit_str = lp_parm_const_string(
+		SNUM(handle->conn), MODULE,
+		"sizelimit", "20");
+
 	module_sizelimit = (off_t) lp_parm_int(SNUM(handle->conn),
 					MODULE, "sizelimit", 20);
-	/* convert from MiB to byte: */
-	module_sizelimit *= 1048576;
 
+	ok = conv_str_off_t_error(module_sizelimit_str,
+				  &module_sizelimit);
+	if (!ok) {
+		DBG_ERR("%s: Invalid sizelimit configuration. "
+			"Denying access to share.");
+		errno = EINVAL;
+		return -1;
+	}
+	if (module_sizelimit == 0) {
+		DBG_INFO("sizelimit set to zero. "
+			 "Size limit restrictions removed.\n");
+	}
+
 	return 0;
 }
 
@@ -61,7 +104,8 @@ static int copy_reg(const char *source, const char *de
 	if (!S_ISREG (source_stats.st_ex_mode))
 		return -1;
 
-	if (source_stats.st_ex_size > module_sizelimit) {
+	if (module_sizelimit &&
+	    source_stats.st_ex_size > module_sizelimit) {
 		DEBUG(5,
 			("%s: size of %s larger than sizelimit (%lld > %lld), rename prohititted\n",
 			MODULE, source,
