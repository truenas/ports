--- source3/modules/vfs_fruit.c.orig	2020-03-26 04:58:46.000000000 -0400
+++ source3/modules/vfs_fruit.c	2020-08-26 11:23:57.694682000 -0400
@@ -4232,7 +4232,14 @@ static ssize_t fruit_pread_meta_stream(vfs_handle_stru
 	int ret;
 
 	nread = SMB_VFS_NEXT_PREAD(handle, fsp, data, n, offset);
-	if (nread == -1 || nread == n) {
+	if (nread <= 0 || nread == n) {
+		/*
+		 * fruit_meta_open_stream() removes O_CREAT flag
+		 * from xattr open. This results in vfs_streams_xattr
+		 * not generating an FSP extension for the files_struct
+		 * and causes subsequent pread() of stream to return
+		 * nread=0 if pread() occurs before pwrite().
+		 */
 		return nread;
 	}
 
@@ -7002,7 +7009,15 @@ static bool fruit_tmsize_do_dirent(vfs_handle_struct *
 		return true;
 	}
 
-	if (bandsize > SIZE_MAX/nbands) {
+	if (nbands == 0) {
+		/*
+		 * We don't want an empty bands directory to cause a complete failure
+		 */
+		DBG_ERR("Bands directory is empty.\n");
+		return true;
+	}
+
+	if (nbands > SIZE_MAX/nbands) {
 		DBG_ERR("tmsize overflow: bandsize [%zu] nbands [%zu]\n",
 			bandsize, nbands);
 		return false;
