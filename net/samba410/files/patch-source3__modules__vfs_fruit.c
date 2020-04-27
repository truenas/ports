--- source3/modules/vfs_fruit.c.orig	2020-04-27 14:00:44.650427741 -0700
+++ source3/modules/vfs_fruit.c	2020-04-27 14:00:57.111634478 -0700
@@ -7002,7 +7002,15 @@ static bool fruit_tmsize_do_dirent(vfs_h
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
