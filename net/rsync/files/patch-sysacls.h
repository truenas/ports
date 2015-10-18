--- /dev/null	2008-03-01 12:01:41.000000000 -0800
+++ ./lib/sysacls.h	2011-10-20 23:11:53.000000000 -0700
@@ -48,6 +48,7 @@
 #define SMB_ACL_GROUP_OBJ	ACL_GROUP_OBJ
 #define SMB_ACL_OTHER		ACL_OTHER
 #define SMB_ACL_MASK		ACL_MASK
+#define SMB_ACL_EVERYONE	ACL_EVERYONE
 
 #define SMB_ACL_T		acl_t
 
@@ -58,6 +59,11 @@
 
 #define SMB_ACL_TYPE_ACCESS	ACL_TYPE_ACCESS
 #define SMB_ACL_TYPE_DEFAULT	ACL_TYPE_DEFAULT
+#define	SMB_ACL_TYPE_NFS4	ACL_TYPE_NFS4
+
+#define	SMB_ACL_BRAND_UNKNOWN	ACL_BRAND_UNKNOWN
+#define	SMB_ACL_BRAND_POSIX	ACL_BRAND_POSIX
+#define	SMB_ACL_BRAND_NFS4	ACL_BRAND_NFS4
 
 #define SMB_ACL_VALID_NAME_BITS	(4 | 2 | 1)
 #define SMB_ACL_VALID_OBJ_BITS	(4 | 2 | 1)
@@ -292,6 +298,8 @@
 SMB_ACL_T sys_acl_get_file(const char *path_p, SMB_ACL_TYPE_T type);
 SMB_ACL_T sys_acl_get_fd(int fd);
 SMB_ACL_T sys_acl_init(int count);
+int sys_acl_get_brand( SMB_ACL_T the_acl, int *brand_p);
+int sys_acl_get_brand_file( const char *path_p, int *brand_p);
 int sys_acl_create_entry(SMB_ACL_T *pacl, SMB_ACL_ENTRY_T *pentry);
 int sys_acl_set_info(SMB_ACL_ENTRY_T entry, SMB_ACL_TAG_T tagtype, uint32 bits, id_t u_g_id);
 int sys_acl_set_access_bits(SMB_ACL_ENTRY_T entry, uint32 bits);
