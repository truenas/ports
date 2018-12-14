--- source3/modules/zfs_list_snapshots.c.orig	2018-11-09 20:21:22.176646000 -0500
+++ source3/modules/zfs_list_snapshots.c	2018-11-09 20:31:34.438530000 -0500
@@ -0,0 +1,243 @@
+/* shadow_copy_zfs: a shadow copy module for ZFS
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
+#include <stdbool.h>
+#include <talloc.h>
+#include <sys/types.h>
+#include <sys/time.h>
+#include <libzfs.h>
+#include "../lib/util/debug.h"
+#include "smb_macros.h"
+#include "zfs_list_snapshots.h"
+
+#define SHADOW_COPY_ZFS_GMT_FORMAT "@GMT-%Y.%m.%d-%H.%M.%S"
+
+static bool shadow_copy_zfs_is_snapshot_included(struct iter_info *info,
+    const char *snap_name)
+{
+	const char **pattern;
+
+#if 0
+	pattern = info->inclusions;
+	while (*pattern) {
+		if (unix_wild_match(*pattern, snap_name)) {
+			break;
+		}
+		pattern++;
+	}
+
+	if (*info->inclusions && !*pattern) {
+		DEBUG(2,("shadow_copy_zfs_add_snapshot: snapshot %s "
+		    "not in inclusion list\n", snap_name));
+		return false;
+	}
+
+	pattern = info->exclusions;
+	while (*pattern) {
+		if (unix_wild_match(*pattern, snap_name)) {
+			DEBUG(2,("shadow_copy_zfs_add_snapshot: snapshot %s "
+			    "in exclusion list\n", snap_name));
+			return false;
+		}
+		pattern++;
+	}
+#endif
+
+	return true;
+}
+
+static int shadow_copy_zfs_add_snapshot(zfs_handle_t *snap, void *data)
+{
+	struct iter_info *info = (struct iter_info *) data;
+	struct snapshot_entry *entry;
+	const char *snap_name;
+	char ts_buf[20];
+	time_t timestamp_t;
+	struct tm timestamp;
+	int rc;
+	size_t req_mem, name_len;
+
+	/* ignore excluded snapshots */
+	snap_name = strchr(zfs_get_name(snap), '@') + 1;
+
+	if (!shadow_copy_zfs_is_snapshot_included(info, snap_name)) {
+		zfs_close(snap);
+		return 0;
+	}
+
+	/* get creation date */
+	rc = zfs_prop_get(snap, ZFS_PROP_CREATION, ts_buf, sizeof(ts_buf),
+			  NULL, NULL, 0, 1);
+	if (rc != 0) {
+		DEBUG(0,("shadow_copy_zfs_add_snapshot: error getting "
+		    "creation date: %s\n", strerror(errno)));
+
+		zfs_close(snap);
+		return -2;
+	}
+
+	sscanf(ts_buf, "%lu", &timestamp_t);
+
+	/* expand list if necessary */
+	req_mem = sizeof(*info->snapshots) +
+		  info->snapshots->num_entries *
+		  sizeof(info->snapshots->entries[0]);
+
+	if (req_mem > talloc_get_size(info->snapshots)) {
+		req_mem += info->snapshots->num_entries / 2 *
+			   sizeof(info->snapshots->entries[0]);
+		info->snapshots = TALLOC_REALLOC(talloc_parent(info->snapshots),
+						 info->snapshots, req_mem);
+		if (info->snapshots == NULL) {
+			DEBUG(0,("shadow_copy_zfs_add_snapshot: out of memory "
+			    "(requested %zu bytes)\n", req_mem));
+
+			zfs_close(snap);
+			return -2;
+		}
+	}
+
+	/* add entry */
+	name_len = strlen(snap_name);
+
+	entry = talloc_size(info->snapshots, sizeof(*entry) + name_len);
+	if (entry == NULL) {
+		DEBUG(0,("shadow_copy_zfs_add_snapshot: out of memory "
+		    "(requested %lu bytes)\n", sizeof(*entry) + name_len));
+
+		zfs_close(snap);
+		return -2;
+	}
+
+	info->snapshots->entries[info->snapshots->num_entries++] = entry;
+
+	gmtime_r(&timestamp_t, &timestamp);
+	strftime(entry->label, sizeof(entry->label), SHADOW_COPY_ZFS_GMT_FORMAT,
+		 &timestamp);
+
+	strlcpy(entry->name, snap_name, name_len + 1);
+
+	zfs_close(snap);
+	return 0;
+}
+
+/*
+  work out the mountpoint of the filesystem
+ */
+struct snapshot_list *shadow_copy_zfs_list_snapshots(TALLOC_CTX *mem_ctx,
+    const char *fs, const char **inclusions, const char **exclusions)
+{
+	struct snapshot_list *snapshots = NULL;
+	struct iter_info iter_info;
+	size_t initial_size;
+	libzfs_handle_t *libzfs = NULL;
+	zfs_handle_t *zfs = NULL;
+	int rc;
+
+
+	/* initialize our result */
+	initial_size = sizeof(*snapshots) + 10 * sizeof(snapshots->entries[0]);
+	snapshots = talloc_size(mem_ctx, initial_size);
+
+	if (snapshots == NULL) {
+		DEBUG(0,("shadow_copy_zfs_list_snapshots: out of memory"
+		    "(requested %zu bytes)\n", initial_size));
+		goto error;
+	}
+
+	snapshots->mountpoint = NULL;
+	snapshots->num_entries = 0;
+
+	libzfs = libzfs_init();
+
+	if (!libzfs) {
+		DEBUG(0,("shadow_copy_zfs_list_snapshots: error opening "
+		    "libzfs: %s\n", strerror(errno)));
+		goto error;
+	}
+
+	zfs = zfs_open(libzfs, fs, ZFS_TYPE_DATASET);
+
+	if (!zfs) {
+		DEBUG(0,("shadow_copy_zfs_list_snapshots: error opening "
+		    "filesystem '%s': %s\n", fs, strerror(errno)));
+		goto error;
+	}
+
+	/* get mountpoint */
+	snapshots->mountpoint = talloc_size(snapshots, ZFS_MAXPROPLEN + 1);
+
+	if (snapshots->mountpoint == NULL) {
+		DEBUG(0,("shadow_copy_zfs_list_snapshots: out of memory "
+		    "(requested %d bytes)\n",  ZFS_MAXPROPLEN + 1));
+		goto error;
+	}
+
+	rc = zfs_prop_get(zfs, ZFS_PROP_MOUNTPOINT, snapshots->mountpoint,
+			  talloc_get_size(snapshots->mountpoint), NULL, NULL,
+			  0, 0);
+
+	if (rc != 0) {
+		DEBUG(0,("shadow_copy_zfs_list_snapshots: error getting "
+		    "mountpoint for '%s': %s\n", fs, strerror(errno)));
+		goto error;
+	}
+
+	/* get snapshots */
+	iter_info.snapshots = snapshots;
+	iter_info.inclusions = inclusions;
+	iter_info.exclusions = exclusions;
+
+	if (iter_info.inclusions == NULL) {
+		DEBUG(0,("shadow_copy_zfs_list_snapshots: error getting "
+		    "shadow:include parameter\n"));
+		goto error;
+	}
+
+	if (iter_info.exclusions == NULL) {
+		DEBUG(0,("shadow_copy_zfs_list_snapshots: error getting "
+		    "shadow:exclude parameter\n"));
+		goto error;
+	}
+
+	rc = zfs_iter_snapshots(zfs, false, shadow_copy_zfs_add_snapshot, &iter_info);
+
+	if (rc != 0) {
+		DEBUG(0,("shadow_copy_zfs_list_snapshots: error getting "
+		    "snapshots for '%s': %s\n", fs, strerror(errno)));
+		goto error;
+	}
+
+	snapshots = iter_info.snapshots;
+	time(&snapshots->timestamp);
+
+	goto done;
+
+error:
+	TALLOC_FREE(snapshots);
+
+done:
+	if (zfs)
+		zfs_close(zfs);
+	if (libzfs)
+		libzfs_fini(libzfs);
+
+	return snapshots;
+}
