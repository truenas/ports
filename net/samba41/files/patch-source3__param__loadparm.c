--- source3/param/loadparm.c	2015-08-31 23:45:32.000000000 -0700
+++ source3/param/loadparm.c	2015-11-23 22:24:03.000000000 -0800
@@ -896,6 +896,7 @@
 	Globals.bLogWriteableFilesOnExit = false;
 	Globals.bCreateKrb5Conf = true;
 	Globals.winbindMaxDomainConnections = 1;
+	Globals.ads_dns_update = 1;
 
 	/* hostname lookups can be very expensive and are broken on
 	   a large number of sites (tridge) */
@@ -3174,10 +3175,12 @@
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
