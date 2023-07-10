--- agent/mibgroup/host/hr_filesys.c.orig	2023-07-07 11:38:33.469110000 -0700
+++ agent/mibgroup/host/hr_filesys.c	2023-07-07 11:54:01.206511000 -0700
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
 #if HAVE_GETFSSTAT
 #if defined(HAVE_STATVFS) && defined(__NetBSD__)
     fscount = getvfsstat(NULL, 0, ST_NOWAIT);
