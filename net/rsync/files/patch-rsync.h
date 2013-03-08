--- /dev/null	2011-10-20 23:09:29.000000000 -0700
+++ ./rsync.h	2011-10-20 23:11:53.000000000 -0700
@@ -954,13 +954,22 @@
 #ifdef SUPPORT_ACLS
     struct rsync_acl *acc_acl; /* access ACL */
     struct rsync_acl *def_acl; /* default ACL */
+    struct nfs4_acl *nfs4_acl; /* NFSv4 ACL */
+    int brand;
 #endif
 #ifdef SUPPORT_XATTRS
     item_list *xattr;
 #endif
 } stat_x;
 
-#define ACL_READY(sx) ((sx).acc_acl != NULL)
+#ifdef SUPPORT_ACLS
+#include "lib/sysacls.h"
+#endif
+
+#define ACL_READY_POSIX(sx) ((sx).acc_acl != NULL)
+#define ACL_READY_NFS4(sx) ((sx).nfs4_acl != NULL)
+#define ACL_READY(sx) (((sx).brand == SMB_ACL_BRAND_NFS4) ? (ACL_READY_NFS4(sx)) : (ACL_READY_POSIX(sx)))
+
 #define XATTR_READY(sx) ((sx).xattr != NULL)
 
 #include "proto.h"
