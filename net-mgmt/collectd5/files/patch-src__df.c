--- src/df.c.orig	2017-06-06 18:13:54 UTC
+++ src/df.c
@@ -28,6 +28,11 @@
 #include "utils/ignorelist/ignorelist.h"
 #include "utils/mount/mount.h"
 
+#ifdef __FreeBSD__
+/* We want to use statfs on FreeBSD, for the mount flags. */
+#undef HAVE_STATVFS
+#endif
+
 #if HAVE_STATVFS
 #if HAVE_SYS_STATVFS_H
 #include <sys/statvfs.h>
@@ -228,7 +233,11 @@ static int df_read(void) {
       }
     }
 
+#ifdef __FreeBSD__
+    if ((statbuf.f_flags & MNT_IGNORE) || (statbuf.f_blocks == 0))
+#else
     if (!statbuf.f_blocks)
+#endif
       continue;
 
     if (by_device) {
@@ -315,14 +324,22 @@ static int df_read(void) {
       uint64_t inode_used;
 
       /* Sanity-check for the values in the struct */
+#ifndef __FreeBSD__ 
       if (statbuf.f_ffree < statbuf.f_favail)
         statbuf.f_ffree = statbuf.f_favail;
+#endif
       if (statbuf.f_files < statbuf.f_ffree)
         statbuf.f_files = statbuf.f_ffree;
 
+#ifdef __FreeBSD__
+      inode_free = (uint64_t)statbuf.f_ffree;
+      inode_reserved = 0;
+      inode_used = (uint64_t)(statbuf.f_files - inode_free);
+#else
       inode_free = (uint64_t)statbuf.f_favail;
       inode_reserved = (uint64_t)(statbuf.f_ffree - statbuf.f_favail);
       inode_used = (uint64_t)(statbuf.f_files - statbuf.f_ffree);
+#endif
 
       if (values_percentage) {
         if (statbuf.f_files > 0) {
