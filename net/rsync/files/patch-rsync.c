--- /dev/null	2011-10-20 23:09:29.000000000 -0700
+++ ./rsync.c	2011-10-20 23:11:53.000000000 -0700
@@ -466,6 +466,8 @@
 		}
 #ifdef SUPPORT_ACLS
 		sx2.acc_acl = sx2.def_acl = NULL;
+		sx2.nfs4_acl = NULL;
+		sys_acl_get_brand_file(fname, &sx2.brand);
 #endif
 #ifdef SUPPORT_XATTRS
 		sx2.xattr = NULL;
@@ -573,10 +575,12 @@
 	 * If set_acl() changes permission bits in the process of setting
 	 * an access ACL, it changes sxp->st.st_mode so we know whether we
 	 * need to chmod(). */
+/*
 	if (preserve_acls && !S_ISLNK(new_mode)) {
 		if (set_acl(fname, file, sxp, new_mode) > 0)
 			updated = 1;
 	}
+*/
 #endif
 
 #ifdef HAVE_CHMOD
@@ -611,6 +615,13 @@
 	}
 #endif
 
+#ifdef SUPPORT_ACLS
+	if (preserve_acls && !S_ISLNK(new_mode)) {
+		if (set_acl(fname, file, sxp, new_mode) > 0)
+			updated = 1;
+	}
+#endif
+
 	if (verbose > 1 && flags & ATTRS_REPORT) {
 		if (updated)
 			rprintf(FCLIENT, "%s\n", fname);
