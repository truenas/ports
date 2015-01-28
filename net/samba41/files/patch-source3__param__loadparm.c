--- ./source3/param/loadparm.c.orig	2015-01-28 00:32:27.759378400 -0800
+++ ./source3/param/loadparm.c	2015-01-28 00:54:41.125286268 -0800
@@ -3170,9 +3170,11 @@
 
 	/* if it's already been set by the command line, then we don't
 	   override here */
+	/*
 	if (parm_table[parmnum].flags & FLAG_CMDLINE) {
 		return true;
 	}
+	*/
 
 	if (parm_table[parmnum].flags & FLAG_DEPRECATED) {
 		DEBUG(1, ("WARNING: The \"%s\" option is deprecated\n",
