--- src/df.c.orig	2017-06-06 18:13:54 UTC
+++ src/df.c
@@ -28,37 +28,49 @@
 #include "utils_ignorelist.h"
 #include "utils_mount.h"
 
-#if HAVE_STATVFS
-#if HAVE_SYS_STATVFS_H
-#include <sys/statvfs.h>
+#include <sys/param.h>
+#include <sys/mount.h>
+#ifdef HAVE_LIBZFS
+#ifdef HAVE_LIBZFS_H
+#include <libzfs.h>
 #endif
-#define STATANYFS statvfs
-#define STATANYFS_STR "statvfs"
-#define BLOCKSIZE(s) ((s).f_frsize ? (s).f_frsize : (s).f_bsize)
-#elif HAVE_STATFS
-#if HAVE_SYS_STATFS_H
-#include <sys/statfs.h>
 #endif
-#define STATANYFS statfs
-#define STATANYFS_STR "statfs"
-#define BLOCKSIZE(s) (s).f_bsize
-#else
-#error "No applicable input method."
+
+
+#ifdef HAVE_LIBZFS
+/* ZFS specific data */
+struct statzfs {
+  uint64_t z_available;
+  uint64_t z_usedbysnapshots;
+  uint64_t z_usedbydataset;
+  uint64_t z_usedbychildren;
+  uint64_t z_usedbyrefreservation;
+};
+#endif
+
+struct mntstat {
+  struct statfs m_fs;
+#ifdef HAVE_LIBZFS
+  struct statzfs m_zfs;
 #endif
+};
 
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
@@ -67,6 +79,8 @@ static int df_init(void) {
     il_mountpoint = ignorelist_create(1);
   if (il_fstype == NULL)
     il_fstype = ignorelist_create(1);
+  if (il_errors == NULL)
+    il_errors = ignorelist_create(0);
 
   return (0);
 }
@@ -123,11 +137,73 @@ static int df_config(const char *key, co
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
 }
@@ -123,6 +133,13 @@ static int df_config(const char *key, co
       values_percentage = 0;
 
+int mntstat(const char *restrict path, struct mntstat *restrict buf)
+{
+  struct statfs *fsp;
+#ifdef HAVE_LIBZFS
+  struct statzfs *zsp;
+  libzfs_handle_t *libzfsp;
+  zfs_handle_t *zfsp;
+  uint64_t available, real_used, total;
+  size_t blocksize;
+#endif
+
+  if (path == NULL || buf == NULL)
+    return (-1);
+
+  fsp = &buf->m_fs;
+  if (statfs(path, fsp) < 0)
+    return (-1);
+
+#ifdef HAVE_LIBZFS
+  if ((libzfsp = libzfs_init()) == NULL)
+    return (-1);
+
+  libzfs_print_on_error(libzfsp, B_TRUE);
+
+  zfsp = zfs_path_to_zhandle(libzfsp, (char *)path,
+    ZFS_TYPE_VOLUME|ZFS_TYPE_DATASET|ZFS_TYPE_FILESYSTEM);
+  if (zfsp == NULL)
+    return (-1);
+
+  zsp = &buf->m_zfs;
+  blocksize = fsp->f_bsize = zfs_prop_get_int(zfsp, ZFS_PROP_RECORDSIZE);
+  available = zsp->z_available = zfs_prop_get_int(zfsp, ZFS_PROP_AVAILABLE);
+  zsp->z_usedbysnapshots = zfs_prop_get_int(zfsp, ZFS_PROP_USEDSNAP);
+  zsp->z_usedbydataset = zfs_prop_get_int(zfsp, ZFS_PROP_USEDDS);
+  zsp->z_usedbychildren = zfs_prop_get_int(zfsp, ZFS_PROP_USEDCHILD);
+  zsp->z_usedbyrefreservation = zfs_prop_get_int(zfsp, ZFS_PROP_USEDREFRESERV);
+
+  zfs_close(zfsp);
+  libzfs_fini(libzfsp);
+
+  real_used = zsp->z_usedbysnapshots + zsp->z_usedbydataset + zsp->z_usedbychildren;
+  total = (available + real_used);
+
+  fsp->f_bsize = blocksize;
+  fsp->f_blocks = total / blocksize;
+  fsp->f_bfree = available / blocksize;
+  fsp->f_bavail = available / blocksize;
+
+  /* Don't support this option when using ZFS */
+  report_inodes = 0;
+#endif
+
+  return (0);
+}
+
 __attribute__((nonnull(2))) static void df_submit_one(char *plugin_instance,
                                                       const char *type,
                                                       const char *type_instance,
@@ -147,13 +223,12 @@ __attribute__((nonnull(2))) static void 
 } /* void df_submit_one */
 
 static int df_read(void) {
-#if HAVE_STATVFS
-  struct statvfs statbuf;
-#elif HAVE_STATFS
-  struct statfs statbuf;
-#endif
-  /* struct STATANYFS statbuf; */
   cu_mount_t *mnt_list;
+  struct mntstat statbuf;
+  struct statfs *fsp = &statbuf.m_fs;
+#ifdef HAVE_LIBZFS
+  struct statzfs *zsp = &statbuf.m_zfs;
+#endif
 
   mnt_list = NULL;
   if (cu_mount_getlist(&mnt_list) == NULL) {
@@ -201,14 +276,26 @@ static int df_read(void) {
     if (dup_ptr != NULL)
       continue;
 
-    if (STATANYFS(mnt_ptr->dir, &statbuf) < 0) {
-      char errbuf[1024];
-      ERROR(STATANYFS_STR "(%s) failed: %s", mnt_ptr->dir,
-            sstrerror(errno, errbuf, sizeof(errbuf)));
+    if (mntstat(mnt_ptr->dir, &statbuf) < 0) {
+      if (log_once == 0 || ignorelist_match(il_errors, mnt_ptr->dir) == 0)
+      {
+        if (log_once == 1)
+        {
+          ignorelist_add(il_errors, mnt_ptr->dir);
+        }
+        char errbuf[1024];
+        ERROR("mntstat(%s) failed: %s", mnt_ptr->dir,
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
+    if ((fsp->f_flags & MNT_IGNORE) || (fsp->f_blocks == 0))
       continue;
 
     if (by_device) {
@@ -237,34 +324,33 @@ static int df_read(void) {
       }
     }
 
-    blocksize = BLOCKSIZE(statbuf);
+    blocksize = fsp->f_bsize;
+
+    /*
+     * Sanity-check for the values in the struct
+     */
+
+    /* Check for negative "available" byes. For example UFS can
+     * report negative free space for user. Notice. blk_reserved
+     * will start to diminish after this. */
+
+    if (fsp->f_bavail < 0)
+      fsp->f_bavail = 0;
 
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
-    if (statbuf.f_bavail < 0)
-      statbuf.f_bavail = 0;
-#endif
     /* Make sure that f_blocks >= f_bfree >= f_bavail */
-    if (statbuf.f_bfree < statbuf.f_bavail)
-      statbuf.f_bfree = statbuf.f_bavail;
-    if (statbuf.f_blocks < statbuf.f_bfree)
-      statbuf.f_blocks = statbuf.f_bfree;
+    if (fsp->f_bfree < fsp->f_bavail)
+      fsp->f_bfree = fsp->f_bavail;
+    if (fsp->f_blocks < fsp->f_bfree)
+      fsp->f_blocks = fsp->f_bfree;
 
-    blk_free = (uint64_t)statbuf.f_bavail;
-    blk_reserved = (uint64_t)(statbuf.f_bfree - statbuf.f_bavail);
-    blk_used = (uint64_t)(statbuf.f_blocks - statbuf.f_bfree);
+    blk_free = (uint64_t)fsp->f_bavail;
+#ifdef HAVE_LIBZFS
+    blk_reserved = (zsp->z_usedbyrefreservation > 0) ?
+      (uint64_t) zsp->z_usedbyrefreservation / blocksize : 0;
+#else
+    blk_reserved = (uint64_t)(fsp->f_bfree - fsp->f_bavail);
+#endif
+    blk_used = (uint64_t)(fsp->f_blocks - fsp->f_bfree);
 
     if (values_absolute) {
       df_submit_one(disk_name, "df_complex", "free",
@@ -276,45 +362,43 @@ static int df_read(void) {
     }
 
     if (values_percentage) {
-      if (statbuf.f_blocks > 0) {
+      if (fsp->f_blocks > 0) {
         df_submit_one(disk_name, "percent_bytes", "free",
-                      (gauge_t)((float_t)(blk_free) / statbuf.f_blocks * 100));
+                      (gauge_t)((float_t)(blk_free) / fsp->f_blocks * 100));
         df_submit_one(
             disk_name, "percent_bytes", "reserved",
-            (gauge_t)((float_t)(blk_reserved) / statbuf.f_blocks * 100));
+            (gauge_t)((float_t)(blk_reserved) / fsp->f_blocks * 100));
         df_submit_one(disk_name, "percent_bytes", "used",
-                      (gauge_t)((float_t)(blk_used) / statbuf.f_blocks * 100));
+                      (gauge_t)((float_t)(blk_used) / fsp->f_blocks * 100));
       } else
         return (-1);
     }
 
     /* inode handling */
-    if (report_inodes && statbuf.f_files != 0 && statbuf.f_ffree != 0) {
+    if (report_inodes && fsp->f_files != 0 && fsp->f_ffree != 0) {
       uint64_t inode_free;
       uint64_t inode_reserved;
       uint64_t inode_used;
 
       /* Sanity-check for the values in the struct */
-      if (statbuf.f_ffree < statbuf.f_favail)
-        statbuf.f_ffree = statbuf.f_favail;
-      if (statbuf.f_files < statbuf.f_ffree)
-        statbuf.f_files = statbuf.f_ffree;
+      if (fsp->f_files < fsp->f_ffree)
+        fsp->f_files = fsp->f_ffree;
 
-      inode_free = (uint64_t)statbuf.f_favail;
-      inode_reserved = (uint64_t)(statbuf.f_ffree - statbuf.f_favail);
-      inode_used = (uint64_t)(statbuf.f_files - statbuf.f_ffree);
+      inode_free = (uint64_t)fsp->f_ffree;
+      inode_reserved = 0;
+      inode_used = (uint64_t)(fsp->f_files - inode_free);
 
       if (values_percentage) {
-        if (statbuf.f_files > 0) {
+        if (fsp->f_files > 0) {
           df_submit_one(
               disk_name, "percent_inodes", "free",
-              (gauge_t)((float_t)(inode_free) / statbuf.f_files * 100));
+              (gauge_t)((float_t)(inode_free) / fsp->f_files * 100));
           df_submit_one(
               disk_name, "percent_inodes", "reserved",
-              (gauge_t)((float_t)(inode_reserved) / statbuf.f_files * 100));
+              (gauge_t)((float_t)(inode_reserved) / fsp->f_files * 100));
           df_submit_one(
               disk_name, "percent_inodes", "used",
-              (gauge_t)((float_t)(inode_used) / statbuf.f_files * 100));
+              (gauge_t)((float_t)(inode_used) / fsp->f_files * 100));
         } else
           return (-1);
       }
