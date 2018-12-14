--- source3/modules/zfs_list_snapshots.h.orig	2018-11-09 19:36:54.187119000 -0500
+++ source3/modules/zfs_list_snapshots.h	2018-11-09 19:36:43.538209000 -0500
@@ -0,0 +1,53 @@
+/*
+ * shadow_copy_zfs: a shadow copy module for ZFS
+ *
+ * Copyright (C) XStor Systems Inc 2011
+ * Copyright (C) iXsystems Inc     2016
+ *
+ * This program is free software; you can redistribute it and/or modify
+ * it under the terms of the GNU General Public License as published by
+ * the Free Software Foundation; either version 2 of the License, or
+ * (at your option) any later version.
+ *
+ * This program is distributed in the hope that it will be useful,
+ * but WITHOUT ANY WARRANTY; without even the implied warranty of
+ * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
+ * GNU General Public License for more details.
+ *
+ * You should have received a copy of the GNU General Public License
+ * along with this program; if not, write to the Free Software
+ * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
+ */
+
+
+#ifndef __ZFS_LIST_SNAPSHOTS_H
+#define __ZFS_LIST_SNAPSHOTS_H
+
+struct iter_info
+{
+    struct snapshot_list *snapshots;
+    const char **inclusions;
+    const char **exclusions;
+};
+
+struct snapshot_entry
+{
+    char label[25];
+    char name[1];
+};
+
+struct snapshot_list
+{
+    time_t timestamp;
+    char *mountpoint;
+    int (*cmpfunc)(const void *, const void *);
+    size_t num_entries;
+    struct snapshot_entry *entries[1];
+};
+
+struct snapshot_list *shadow_copy_zfs_list_snapshots(TALLOC_CTX *mem_ctx,
+    const char *fs, const char **inclusions, const char **exclusions);
+
+
+#endif	/* __ZFS_LIST_SNAPSHOTS_H */
+
