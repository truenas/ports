--- src/df.c.orig	2017-06-06 18:13:54 UTC
+++ src/df.c
@@ -28,37 +28,26 @@
 #include "utils_ignorelist.h"
 #include "utils_mount.h"
 
-#if HAVE_STATVFS
-#if HAVE_SYS_STATVFS_H
-#include <sys/statvfs.h>
-#endif
-#define STATANYFS statvfs
-#define STATANYFS_STR "statvfs"
-#define BLOCKSIZE(s) ((s).f_frsize ? (s).f_frsize : (s).f_bsize)
-#elif HAVE_STATFS
-#if HAVE_SYS_STATFS_H
-#include <sys/statfs.h>
-#endif
-#define STATANYFS statfs
-#define STATANYFS_STR "statfs"
-#define BLOCKSIZE(s) (s).f_bsize
-#else
-#error "No applicable input method."
-#endif
+#include <sys/param.h>
+#include <sys/mount.h>
+
 
 static const char *config_keys[] = {
     "Device",         "MountPoint",   "FSType",         "IgnoreSelected",
-    "ReportByDevice", "ReportInodes", "ValuesAbsolute", "ValuesPercentage"};
+    "ReportByDevice", "ReportInodes", "ValuesAbsolute", "ValuesPercentage",
+    "LogOnce"};
 static int config_keys_num = STATIC_ARRAY_SIZE(config_keys);
 
 static ignorelist_t *il_device = NULL;
 static ignorelist_t *il_mountpoint = NULL;
 static ignorelist_t *il_fstype = NULL;
+static ignorelist_t *il_errors = NULL;
 
 static _Bool by_device = 0;
 static _Bool report_inodes = 0;
 static _Bool values_absolute = 1;
 static _Bool values_percentage = 0;
+static _Bool log_once = 0;
 
 static int df_init(void) {
   if (il_device == NULL)
@@ -67,6 +56,8 @@ static int df_init(void) {
     il_mountpoint = ignorelist_create(1);
   if (il_fstype == NULL)
     il_fstype = ignorelist_create(1);
+  if (il_errors == NULL)
+    il_errors = ignorelist_create(0);
 
   return (0);
 }
@@ -123,6 +114,13 @@ static int df_config(const char *key, co
       values_percentage = 0;
 
     return (0);
+  } else if (strcasecmp(key, "LogOnce") == 0) {
+    if (IS_TRUE(value))
+      log_once = 1;
+    else
+      log_once = 0;
+
+    return (0);
   }
 
   return (-1);
@@ -147,13 +145,8 @@ __attribute__((nonnull(2))) static void 
 } /* void df_submit_one */
 
 static int df_read(void) {
-#if HAVE_STATVFS
-  struct statvfs statbuf;
-#elif HAVE_STATFS
-  struct statfs statbuf;
-#endif
-  /* struct STATANYFS statbuf; */
   cu_mount_t *mnt_list;
+  struct statfs statbuf;
 
   mnt_list = NULL;
   if (cu_mount_getlist(&mnt_list) == NULL) {
@@ -163,7 +156,7 @@ static int df_read(void) {
 
   for (cu_mount_t *mnt_ptr = mnt_list; mnt_ptr != NULL;
        mnt_ptr = mnt_ptr->next) {
-    unsigned long long blocksize;
+    uint64_t blocksize;
     char disk_name[256];
     cu_mount_t *dup_ptr;
     uint64_t blk_free;
@@ -201,14 +194,26 @@ static int df_read(void) {
     if (dup_ptr != NULL)
       continue;
 
-    if (STATANYFS(mnt_ptr->dir, &statbuf) < 0) {
-      char errbuf[1024];
-      ERROR(STATANYFS_STR "(%s) failed: %s", mnt_ptr->dir,
-            sstrerror(errno, errbuf, sizeof(errbuf)));
+    if (statfs(mnt_ptr->dir, &statbuf) < 0) {
+      if (log_once == 0 || ignorelist_match(il_errors, mnt_ptr->dir) == 0)
+      {
+        if (log_once == 1)
+        {
+          ignorelist_add(il_errors, mnt_ptr->dir);
+        }
+        char errbuf[1024];
+        ERROR("statfs(%s) failed: %s", mnt_ptr->dir,
+              sstrerror(errno, errbuf, sizeof(errbuf)));
+      }
       continue;
+    } else {
+      if (log_once == 1)
+      {
+        ignorelist_remove(il_errors, mnt_ptr->dir);
+      }
     }
 
-    if (!statbuf.f_blocks)
+    if ((statbuf.f_flags & MNT_IGNORE) || (statbuf.f_blocks == 0))
       continue;
 
     if (by_device) {
@@ -237,25 +242,19 @@ static int df_read(void) {
       }
     }
 
-    blocksize = BLOCKSIZE(statbuf);
+    blocksize = statbuf.f_bsize;
+
+    /*
+     * Sanity-check for the values in the struct
+     */
+
+    /* Check for negative "available" byes. For example UFS can
+     * report negative free space for user. Notice. blk_reserved
+     * will start to diminish after this. */
 
-/*
- * Sanity-check for the values in the struct
- */
-/* Check for negative "available" byes. For example UFS can
- * report negative free space for user. Notice. blk_reserved
- * will start to diminish after this. */
-#if HAVE_STATVFS
-    /* Cast and temporary variable are needed to avoid
-     * compiler warnings.
-     * ((struct statvfs).f_bavail is unsigned (POSIX)) */
-    int64_t signed_bavail = (int64_t)statbuf.f_bavail;
-    if (signed_bavail < 0)
-      statbuf.f_bavail = 0;
-#elif HAVE_STATFS
     if (statbuf.f_bavail < 0)
       statbuf.f_bavail = 0;
-#endif
+
     /* Make sure that f_blocks >= f_bfree >= f_bavail */
     if (statbuf.f_bfree < statbuf.f_bavail)
       statbuf.f_bfree = statbuf.f_bavail;
@@ -295,14 +294,12 @@ static int df_read(void) {
       uint64_t inode_used;
 
       /* Sanity-check for the values in the struct */
-      if (statbuf.f_ffree < statbuf.f_favail)
-        statbuf.f_ffree = statbuf.f_favail;
       if (statbuf.f_files < statbuf.f_ffree)
         statbuf.f_files = statbuf.f_ffree;
 
-      inode_free = (uint64_t)statbuf.f_favail;
-      inode_reserved = (uint64_t)(statbuf.f_ffree - statbuf.f_favail);
-      inode_used = (uint64_t)(statbuf.f_files - statbuf.f_ffree);
+      inode_free = (uint64_t)statbuf.f_ffree;
+      inode_reserved = 0;
+      inode_used = (uint64_t)(statbuf.f_files - inode_free);
 
       if (values_percentage) {
         if (statbuf.f_files > 0) {
