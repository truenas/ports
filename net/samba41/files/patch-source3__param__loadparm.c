--- ./source3/param/loadparm.c
+++ ./source3/param/loadparm.c
@@ -3174,10 +3174,12 @@ bool lp_do_parameter(int snum, const char *pszParmName, const char *pszParmValue
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
