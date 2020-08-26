--- source3/modules/vfs_fruit.c.orig	2020-08-26 12:43:26.095459000 -0400
+++ source3/modules/vfs_fruit.c	2020-08-26 12:43:32.741657000 -0400
@@ -2193,7 +2193,14 @@ static ssize_t fruit_pread_meta_stream(vfs_handle_stru
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
 
