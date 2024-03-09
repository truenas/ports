--- agent/mibgroup/host/hr_filesys.c.orig	2024-03-09 05:55:28.685425000 -0600
+++ agent/mibgroup/host/hr_filesys.c	2024-03-09 05:57:23.262165000 -0600
@@ -618,6 +618,13 @@ static FILE    *fp;
 void
 Init_HR_FileSys(void)
 {
+#if defined(HAVE_GETMNTINFO)
+    struct stat statbuf;
+    struct statfs *mntbuf;
+
+    if (0 == stat("/usr/local/etc/snmp/enable_getmntinfo", &statbuf))
+        getmntinfo(&mntbuf, MNT_WAIT);
+#endif
 #ifdef HAVE_GETFSSTAT
 #if defined(HAVE_STATVFS) && defined(__NetBSD__)
     fscount = getvfsstat(NULL, 0, ST_NOWAIT);
