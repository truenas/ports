--- source3/modules/nfs4_acls.h.orig	2019-08-26 09:24:45.836742432 -0700
+++ source3/modules/nfs4_acls.h	2019-08-26 09:24:53.004804541 -0700
@@ -118,6 +118,7 @@ struct smbacl4_vfs_params {
 	bool do_chown;
 	enum smbacl4_acedup_enum acedup;
 	bool map_full_control;
+	bool sort_aces;
 };
 
 int smbacl4_get_vfs_params(struct connection_struct *conn,
