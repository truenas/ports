--- src/df.c.orig	2017-01-23 07:53:57 UTC
+++ src/df.c
@@ -46,19 +46,55 @@
 #error "No applicable input method."
 #endif
 
+#if HAVE_LIBZFS
+# if HAVE_LIBZFS_H
+#  include <libzfs.h>
+# endif
+# undef STATANYFS
+# undef STATANYFS_STR
+# define STATANYFS statzfs
+# define STATANYFS_STR "statzfs"
+# undef BLOCKSIZE
+# define BLOCKSIZE(s) (s).f_bsize
+struct statzfs {
+   /* Hacky. This is a copy of struct statvfs */
+   fsblkcnt_t  f_bavail;   /* Number of blocks */
+   fsblkcnt_t  f_bfree;
+   fsblkcnt_t  f_blocks;
+   fsfilcnt_t  f_favail;   /* Number of files (e.g., inodes) */
+   fsfilcnt_t  f_ffree;
+   fsfilcnt_t  f_files;
+   unsigned long   f_bsize;    /* Size of blocks counted above */
+   unsigned long   f_flag;
+   unsigned long   f_frsize;   /* Size of fragments */
+   unsigned long   f_fsid;     /* Not meaningful */
+   unsigned long   f_namemax;  /* Same as pathconf(_PC_NAME_MAX) */
+
+   /* ZFS specific data */
+   uint64_t    f_available;
+   uint64_t    f_usedbysnapshots;
+   uint64_t    f_usedbydataset;
+   uint64_t    f_usedbychildren;
+   uint64_t    f_usedbyrefreservation;
+};
+#endif
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
@@ -67,6 +103,8 @@
     il_mountpoint = ignorelist_create(1);
   if (il_fstype == NULL)
     il_fstype = ignorelist_create(1);
+  if (il_errors == NULL)
+    il_errors = ignorelist_create(0);
 
   return (0);
 }
@@ -123,11 +161,68 @@
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
 
+#if HAVE_LIBZFS
+int
+statzfs(const char *restrict path, struct statzfs *restrict buf)
+{
+   size_t blocksize = 1024;
+   libzfs_handle_t *libzfsp;
+   zfs_handle_t *zfsp;
+   uint64_t available, real_used, total;
+
+   if (path == NULL)
+       return (-1);
+
+   if (statvfs(path, (struct statvfs *)buf) < 0)
+       return (-1);
+
+   if ((libzfsp = libzfs_init()) == NULL)
+       return (-1);
+
+   libzfs_print_on_error(libzfsp, B_TRUE);
+
+   zfsp = zfs_path_to_zhandle(libzfsp, (char *)path,
+       ZFS_TYPE_VOLUME|ZFS_TYPE_DATASET|ZFS_TYPE_FILESYSTEM);
+   if (zfsp == NULL)
+       return (-1);
+
+   buf->f_available = zfs_prop_get_int(zfsp, ZFS_PROP_AVAILABLE);
+   buf->f_usedbysnapshots = zfs_prop_get_int(zfsp, ZFS_PROP_USEDSNAP);
+   buf->f_usedbydataset = zfs_prop_get_int(zfsp, ZFS_PROP_USEDDS);
+   buf->f_usedbychildren = zfs_prop_get_int(zfsp, ZFS_PROP_USEDCHILD);
+   buf->f_usedbyrefreservation = zfs_prop_get_int(zfsp, ZFS_PROP_USEDREFRESERV);
+
+   zfs_close(zfsp);
+   libzfs_fini(libzfsp);
+
+   real_used = buf->f_usedbysnapshots + buf->f_usedbydataset + buf->f_usedbychildren;
+   total = (real_used + buf->f_available);
+   available = buf->f_available;
+
+   buf->f_bsize = blocksize;
+   buf->f_blocks = total / blocksize;
+   buf->f_bfree = available / blocksize;
+   buf->f_bavail = available / blocksize;
+
+   /* Don't support this option when using ZFS */
+   report_inodes = 0;
+
+   return (0);
+}
+#endif
+
 __attribute__((nonnull(2))) static void df_submit_one(char *plugin_instance,
                                                       const char *type,
                                                       const char *type_instance,
@@ -147,7 +242,9 @@
 } /* void df_submit_one */
 
 static int df_read(void) {
-#if HAVE_STATVFS
+#if HAVE_LIBZFS
+  struct statzfs statbuf;
+#elif HAVE_STATVFS
   struct statvfs statbuf;
 #elif HAVE_STATFS
   struct statfs statbuf;
@@ -202,10 +299,22 @@
       continue;
 
     if (STATANYFS(mnt_ptr->dir, &statbuf) < 0) {
-      char errbuf[1024];
-      ERROR(STATANYFS_STR "(%s) failed: %s", mnt_ptr->dir,
-            sstrerror(errno, errbuf, sizeof(errbuf)));
+      if (log_once == 0 || ignorelist_match(il_errors, mnt_ptr->dir) == 0)
+      {
+        if (log_once == 1)
+        {
+          ignorelist_add(il_errors, mnt_ptr->dir);
+        }
+        char errbuf[1024];
+        ERROR(STATANYFS_STR "(%s) failed: %s", mnt_ptr->dir,
+              sstrerror(errno, errbuf, sizeof(errbuf)));
+      }
       continue;
+    } else {
+      if (log_once == 1)
+      {
+        ignorelist_remove(il_errors, mnt_ptr->dir);
+      }
     }
 
     if (!statbuf.f_blocks)
@@ -263,7 +372,13 @@
       statbuf.f_blocks = statbuf.f_bfree;
 
     blk_free = (uint64_t)statbuf.f_bavail;
+#if HAVE_LIBZFS
+    blk_reserved = 0;
+    if (statbuf.f_usedbyrefreservation > 0)
+      blk_reserved = (uint64_t) statbuf.f_usedbyrefreservation / blocksize;
+#else
     blk_reserved = (uint64_t)(statbuf.f_bfree - statbuf.f_bavail);
+#endif
     blk_used = (uint64_t)(statbuf.f_blocks - statbuf.f_bfree);
 
     if (values_absolute) {
