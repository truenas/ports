--- source3/param/loadparm.c.orig	2015-01-28 00:32:27.759378400 -0800
+++ source3/param/loadparm.c	2015-01-31 15:04:49.919907614 -0800
@@ -3174,10 +3174,12 @@
 		return true;
 	}
 
+	/*
 	if (parm_table[parmnum].flags & FLAG_DEPRECATED) {
 		DEBUG(1, ("WARNING: The \"%s\" option is deprecated\n",
 			  pszParmName));
 	}
+	*/
 
 	/* we might point at a service, the default service or a global */
 	if (snum < 0) {
