--- /dev/null	2010-06-30 09:17:26.000000000 -0700
+++ ./hlink.c	2011-10-20 23:11:41.000000000 -0700
@@ -373,6 +373,7 @@
 		int j = 0;
 #ifdef SUPPORT_ACLS
 		alt_sx.acc_acl = alt_sx.def_acl = NULL;
+		alt_sx.nfs4_acl = NULL;
 #endif
 #ifdef SUPPORT_XATTRS
 		alt_sx.xattr = NULL;
@@ -411,7 +412,9 @@
 				else {
 					sxp->acc_acl = alt_sx.acc_acl;
 					sxp->def_acl = alt_sx.def_acl;
+					sxp->nfs4_acl = alt_sx.nfs4_acl;
 					alt_sx.acc_acl = alt_sx.def_acl = NULL;
+					alt_sx.nfs4_acl = NULL;
 				}
 			}
 #endif
@@ -501,6 +504,8 @@
 
 #ifdef SUPPORT_ACLS
 	prev_sx.acc_acl = prev_sx.def_acl = NULL;
+	prev_sx.nfs4_acl = NULL;
+	sys_acl_get_brand_file(fname, &prev_sx.brand);
 #endif
 #ifdef SUPPORT_XATTRS
 	prev_sx.xattr = NULL;
