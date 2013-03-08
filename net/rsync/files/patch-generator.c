--- /dev/null	2011-10-20 23:09:29.000000000 -0700
+++ ./generator.c	2011-10-20 23:11:41.000000000 -0700
@@ -1363,6 +1363,8 @@
 
 #ifdef SUPPORT_ACLS
 	sx.acc_acl = sx.def_acl = NULL;
+	sx.nfs4_acl = NULL;
+	sys_acl_get_brand_file(fname, &sx.brand);
 #endif
 #ifdef SUPPORT_XATTRS
 	sx.xattr = NULL;
