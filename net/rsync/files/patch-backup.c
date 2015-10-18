--- /dev/null	2010-07-03 09:19:01.000000000 -0700
+++ ./backup.c	2011-10-20 23:11:41.000000000 -0700
@@ -139,6 +139,8 @@
 			} else {
 #ifdef SUPPORT_ACLS
 				sx.acc_acl = sx.def_acl = NULL;
+				sx.nfs4_acl = NULL;
+				sys_acl_get_brand_file(rel, &sx.brand);
 #endif
 #ifdef SUPPORT_XATTRS
 				sx.xattr = NULL;
@@ -219,6 +221,7 @@
 		return 1;
 #ifdef SUPPORT_ACLS
 	sx.acc_acl = sx.def_acl = NULL;
+	sx.nfs4_acl = NULL;		
 #endif
 #ifdef SUPPORT_XATTRS
 	sx.xattr = NULL;
