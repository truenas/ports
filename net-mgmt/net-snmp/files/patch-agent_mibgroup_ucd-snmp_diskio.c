--- agent/mibgroup/ucd-snmp/diskio.c.orig	2014-12-08 12:23:22.000000000 -0800
+++ agent/mibgroup/ucd-snmp/diskio.c	2021-06-30 04:50:46.738928291 -0700
@@ -820,6 +820,13 @@ void devla_getstats(unsigned int regno, void *dummy) {
                 return;
                 }
 
+        if (ndevs && (ndevs != lastat->dinfo->numdevs)) {
+                ndevs = 0;
+                if (devloads != NULL) {
+                        free(devloads);
+                }
+        }
+
         if (ndevs != 0) {
                 for (i=0; i < ndevs; i++) {
                         snprintf(current_name, sizeof(current_name), "%s%d",
@@ -944,14 +951,14 @@ var_diskio(struct variable * vp,
         return (u_char *) stat->dinfo->devices[indx].device_name;
     case DISKIO_NREAD:
 #if HAVE_DEVSTAT_GETDEVS
-        long_ret = (signed long) stat->dinfo->devices[indx].bytes[DEVSTAT_READ];
+        long_ret = (signed long) stat->dinfo->devices[indx].bytes[DEVSTAT_READ] & 0xFFFFFFFF;
 #else
         long_ret = (signed long) stat->dinfo->devices[indx].bytes_read;
 #endif
         return (u_char *) & long_ret;
     case DISKIO_NWRITTEN:
 #if HAVE_DEVSTAT_GETDEVS
-        long_ret = (signed long) stat->dinfo->devices[indx].bytes[DEVSTAT_WRITE];
+        long_ret = (signed long) stat->dinfo->devices[indx].bytes[DEVSTAT_WRITE] & 0xFFFFFFFF;
 #else
         long_ret = (signed long) stat->dinfo->devices[indx].bytes_written;
 #endif
@@ -978,14 +985,14 @@ var_diskio(struct variable * vp,
         return (u_char *) & c64_ret;
     case DISKIO_READS:
 #if HAVE_DEVSTAT_GETDEVS
-        long_ret = (signed long) stat->dinfo->devices[indx].operations[DEVSTAT_READ];
+        long_ret = (signed long) stat->dinfo->devices[indx].operations[DEVSTAT_READ] & 0xFFFFFFFF;
 #else
         long_ret = (signed long) stat->dinfo->devices[indx].num_reads;
 #endif
         return (u_char *) & long_ret;
     case DISKIO_WRITES:
 #if HAVE_DEVSTAT_GETDEVS
-        long_ret = (signed long) stat->dinfo->devices[indx].operations[DEVSTAT_WRITE];
+        long_ret = (signed long) stat->dinfo->devices[indx].operations[DEVSTAT_WRITE] & 0xFFFFFFFF;
 #else
         long_ret = (signed long) stat->dinfo->devices[indx].num_writes;
 #endif
