--- ./source3/modules/vfs_shadow_copy2.c
+++ ./source3/modules/vfs_shadow_copy2.c
@@ -1,10 +1,11 @@
 /*
- * Third attempt at a shadow copy module
+ * shadow_copy2: a shadow copy module (second implementation)
  *
  * Copyright (C) Andrew Tridgell   2007 (portions taken from shadow_copy2)
  * Copyright (C) Ed Plese          2009
  * Copyright (C) Volker Lendecke   2011
  * Copyright (C) Christian Ambach  2011
+ * Copyright (C) Michael Adam      2013
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
@@ -22,83 +23,10 @@
  */
 
 /*
-
-  This is a 3rd implemetation of a shadow copy module for exposing
-  snapshots to windows clients as shadow copies. This version has the
-  following features:
-
-     1) you don't need to populate your shares with symlinks to the
-     snapshots. This can be very important when you have thousands of
-     shares, or use [homes]
-
-     2) the inode number of the files is altered so it is different
-     from the original. This allows the 'restore' button to work
-     without a sharing violation
-
-     3) shadow copy results can be sorted before being sent to the
-     client.  This is beneficial for filesystems that don't read
-     directories alphabetically (the default unix).
-
-     4) vanity naming for snapshots. Snapshots can be named in any
-     format compatible with str[fp]time conversions.
-
-     5) time stamps in snapshot names can be represented in localtime
-     rather than UTC.
-
-  Module options:
-
-      shadow:snapdir = <directory where snapshots are kept>
-
-      This is the directory containing the @GMT-* snapshot directories. If it is an absolute
-      path it is used as-is. If it is a relative path, then it is taken relative to the mount
-      point of the filesystem that the root of this share is on
-
-      shadow:basedir = <base directory that snapshots are from>
-
-      This is an optional parameter that specifies the directory that
-      the snapshots are relative to. It defaults to the filesystem
-      mount point
-
-      shadow:fixinodes = yes/no
-
-      If you enable shadow:fixinodes then this module will modify the
-      apparent inode number of files in the snapshot directories using
-      a hash of the files path. This is needed for snapshot systems
-      where the snapshots have the same device:inode number as the
-      original files (such as happens with GPFS snapshots). If you
-      don't set this option then the 'restore' button in the shadow
-      copy UI will fail with a sharing violation.
-
-      shadow:sort = asc/desc, or not specified for unsorted (default)
-
-      This is an optional parameter that specifies that the shadow
-      copy directories should be sorted before sending them to the
-      client.  This can be beneficial as unix filesystems are usually
-      not listed alphabetically sorted.  If enabled, you typically
-      want to specify descending order.
-
-      shadow:format = <format specification for snapshot names>
-
-      This is an optional parameter that specifies the format
-      specification for the naming of snapshots.  The format must
-      be compatible with the conversion specifications recognized
-      by str[fp]time.  The default value is "@GMT-%Y.%m.%d-%H.%M.%S".
-
-      shadow:sscanf = yes/no (default is no)
-
-      The time is the unsigned long integer (%lu) in the format string
-      rather than a time strptime() can parse.  The result must be a unix time_t
-      time.
-
-      shadow:localtime = yes/no (default is no)
-
-      This is an optional parameter that indicates whether the
-      snapshot names are in UTC/GMT or the local time.
-
-
-  The following command would generate a correctly formatted directory name
-  for use with the default parameters:
-     date -u +@GMT-%Y.%m.%d-%H.%M.%S
+ * This is a second implemetation of a shadow copy module for exposing
+ * file system snapshots to windows clients as shadow copies.
+ *
+ * See the manual page for documentation.
  */
 
 #include "includes.h"
@@ -107,6 +35,22 @@
 #include <ccan/hash/hash.h>
 #include "util_tdb.h"
 
+struct shadow_copy2_config {
+	char *gmt_format;
+	bool use_sscanf;
+	bool use_localtime;
+	char *snapdir;
+	bool snapdirseverywhere;
+	bool crossmountpoints;
+	bool fixinodes;
+	char *sort_order;
+	bool snapdir_absolute;
+	char *basedir;
+	char *mount_point;
+	char *rel_connectpath; /* share root, relative to the basedir */
+	char *snapshot_basepath; /* the absolute version of snapdir */
+};
+
 static bool shadow_copy2_find_slashes(TALLOC_CTX *mem_ctx, const char *str,
 				      size_t **poffsets,
 				      unsigned *pnum_offsets)
@@ -141,51 +85,143 @@ static bool shadow_copy2_find_slashes(TALLOC_CTX *mem_ctx, const char *str,
 	return true;
 }
 
-static char *shadow_copy2_insert_string(TALLOC_CTX *mem_ctx,
-					struct vfs_handle_struct *handle,
-					time_t snapshot)
+/**
+ * Given a timstamp, build the posix level GTM-tag string
+ * based on the configurable format.
+ */
+static size_t shadow_copy2_posix_gmt_string(struct vfs_handle_struct *handle,
+					    time_t snapshot,
+					    char *snaptime_string,
+					    size_t len)
 {
-	const char *fmt;
 	struct tm snap_tm;
-	fstring snaptime_string;
 	size_t snaptime_len;
+	struct shadow_copy2_config *config;
 
-	fmt = lp_parm_const_string(SNUM(handle->conn), "shadow",
-				   "format", GMT_FORMAT);
+	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy2_config,
+				return 0);
 
-	if (lp_parm_bool(SNUM(handle->conn), "shadow", "sscanf", false)) {
-		snaptime_len = snprintf(snaptime_string, sizeof(snaptime_string), fmt,
-				   (unsigned long)snapshot);
+	if (config->use_sscanf) {
+		snaptime_len = snprintf(snaptime_string,
+					len,
+					config->gmt_format,
+					(unsigned long)snapshot);
 		if (snaptime_len <= 0) {
 			DEBUG(10, ("snprintf failed\n"));
-			return NULL;
+			return snaptime_len;
 		}
 	} else {
-		if (lp_parm_bool(SNUM(handle->conn), "shadow", "localtime", false)) {
+		if (config->use_localtime) {
 			if (localtime_r(&snapshot, &snap_tm) == 0) {
 				DEBUG(10, ("gmtime_r failed\n"));
-				return NULL;
+				return -1;
 			}
 		} else {
 			if (gmtime_r(&snapshot, &snap_tm) == 0) {
 				DEBUG(10, ("gmtime_r failed\n"));
-				return NULL;
+				return -1;
 			}
 		}
-		snaptime_len = strftime(snaptime_string, sizeof(snaptime_string), fmt,
-				   &snap_tm);
+		snaptime_len = strftime(snaptime_string,
+					len,
+					config->gmt_format,
+					&snap_tm);
 		if (snaptime_len == 0) {
 			DEBUG(10, ("strftime failed\n"));
-			return NULL;
+			return 0;
 		}
 	}
-	return talloc_asprintf(mem_ctx, "/%s/%s",
-			       lp_parm_const_string(
-				       SNUM(handle->conn), "shadow", "snapdir",
-				       ".snapshots"),
-			       snaptime_string);
+
+	return snaptime_len;
+}
+
+/**
+ * Given a timstamp, build the string to insert into a path
+ * as a path component for creating the local path to the
+ * snapshot at the given timestamp of the input path.
+ *
+ * In the case of a parallel snapdir (specified with an
+ * absolute path), this is the inital portion of the
+ * local path of any snapshot file. The complete path is
+ * obtained by appending the portion of the file's path
+ * below the share root's mountpoint.
+ */
+static char *shadow_copy2_insert_string(TALLOC_CTX *mem_ctx,
+					struct vfs_handle_struct *handle,
+					time_t snapshot)
+{
+	fstring snaptime_string;
+	size_t snaptime_len = 0;
+	char *result = NULL;
+	struct shadow_copy2_config *config;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy2_config,
+				return NULL);
+
+	snaptime_len = shadow_copy2_posix_gmt_string(handle,
+						     snapshot,
+						     snaptime_string,
+						     sizeof(snaptime_string));
+	if (snaptime_len <= 0) {
+		return NULL;
+	}
+
+	if (config->snapdir_absolute) {
+		result = talloc_asprintf(mem_ctx, "%s/%s",
+					 config->snapdir, snaptime_string);
+	} else {
+		result = talloc_asprintf(mem_ctx, "/%s/%s",
+					 config->snapdir, snaptime_string);
+	}
+	if (result == NULL) {
+		DEBUG(1, (__location__ " talloc_asprintf failed\n"));
+	}
+
+	return result;
+}
+
+/**
+ * Build the posix snapshot path for the connection
+ * at the given timestamp, i.e. the absolute posix path
+ * that contains the snapshot for this file system.
+ *
+ * This only applies to classical case, i.e. not
+ * to the "snapdirseverywhere" mode.
+ */
+static char *shadow_copy2_snapshot_path(TALLOC_CTX *mem_ctx,
+					struct vfs_handle_struct *handle,
+					time_t snapshot)
+{
+	fstring snaptime_string;
+	size_t snaptime_len = 0;
+	char *result = NULL;
+	struct shadow_copy2_config *config;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy2_config,
+				return NULL);
+
+	snaptime_len = shadow_copy2_posix_gmt_string(handle,
+						     snapshot,
+						     snaptime_string,
+						     sizeof(snaptime_string));
+	if (snaptime_len <= 0) {
+		return NULL;
+	}
+
+	result = talloc_asprintf(mem_ctx, "%s/%s",
+				 config->snapshot_basepath, snaptime_string);
+	if (result == NULL) {
+		DEBUG(1, (__location__ " talloc_asprintf failed\n"));
+	}
+
+	return result;
 }
 
+/**
+ * Strip a snapshot component from an filename as
+ * handed in via the smb layer.
+ * Returns the parsed timestamp and the stripped filename.
+ */
 static bool shadow_copy2_strip_snapshot(TALLOC_CTX *mem_ctx,
 					struct vfs_handle_struct *handle,
 					const char *name,
@@ -198,12 +234,19 @@ static bool shadow_copy2_strip_snapshot(TALLOC_CTX *mem_ctx,
 	char *q;
 	char *stripped;
 	size_t rest_len, dst_len;
+	struct shadow_copy2_config *config;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy2_config,
+				return false);
+
+	DEBUG(10, (__location__ ": enter path '%s'\n", name));
 
 	p = strstr_m(name, "@GMT-");
 	if (p == NULL) {
 		goto no_snapshot;
 	}
 	if ((p > name) && (p[-1] != '/')) {
+		/* the GMT-token does not start a path-component */
 		goto no_snapshot;
 	}
 	q = strptime(p, GMT_FORMAT, &tm);
@@ -216,6 +259,7 @@ static bool shadow_copy2_strip_snapshot(TALLOC_CTX *mem_ctx,
 		goto no_snapshot;
 	}
 	if ((p == name) && (q[0] == '\0')) {
+		/* the name consists of only the GMT token */
 		if (pstripped != NULL) {
 			stripped = talloc_strdup(mem_ctx, "");
 			if (stripped == NULL) {
@@ -227,6 +271,14 @@ static bool shadow_copy2_strip_snapshot(TALLOC_CTX *mem_ctx,
 		return true;
 	}
 	if (q[0] != '/') {
+		/*
+		 * The GMT token is either at the end of the path
+		 * or it is not a complete path component, i.e. the
+		 * path component continues after the gmt-token.
+		 *
+		 * TODO: Is this correct? Or would the GMT tag as the
+		 * last component be a valid input?
+		 */
 		goto no_snapshot;
 	}
 	q += 1;
@@ -234,8 +286,7 @@ static bool shadow_copy2_strip_snapshot(TALLOC_CTX *mem_ctx,
 	rest_len = strlen(q);
 	dst_len = (p-name) + rest_len;
 
-	if (lp_parm_bool(SNUM(handle->conn), "shadow", "snapdirseverywhere",
-			 false)) {
+	if (config->snapdirseverywhere) {
 		char *insert;
 		bool have_insert;
 		insert = shadow_copy2_insert_string(talloc_tos(), handle,
@@ -245,11 +296,49 @@ static bool shadow_copy2_strip_snapshot(TALLOC_CTX *mem_ctx,
 			return false;
 		}
 
+		DEBUG(10, (__location__ ": snapdirseverywhere mode.\n"
+			   "path '%s'.\n"
+			   "insert string '%s'\n", name, insert));
+
 		have_insert = (strstr(name, insert+1) != NULL);
-		TALLOC_FREE(insert);
 		if (have_insert) {
+			DEBUG(10, (__location__ ": insert string '%s' found in "
+				   "path '%s' found in snapdirseverywhere mode "
+				   "==> already converted\n", insert, name));
+			TALLOC_FREE(insert);
+			goto no_snapshot;
+		}
+		TALLOC_FREE(insert);
+	} else {
+		char *snapshot_path;
+		char *s;
+
+		snapshot_path = shadow_copy2_snapshot_path(talloc_tos(),
+							   handle,
+							   timestamp);
+		if (snapshot_path == NULL) {
+			errno = ENOMEM;
+			return false;
+		}
+
+		DEBUG(10, (__location__ " path: '%s'.\n"
+			   "snapshot path: '%s'\n", name, snapshot_path));
+
+		s = strstr(name, snapshot_path);
+		if (s == name) {
+			/*
+			 * this starts with "snapshot_basepath/GMT-Token"
+			 * so it is already a converted absolute
+			 * path. Don't process further.
+			 */
+			DEBUG(10, (__location__ ": path '%s' starts with "
+				   "snapshot path '%s' (not in "
+				   "snapdirseverywhere mode) ==> "
+				   "already converted\n", name, snapshot_path));
+			talloc_free(snapshot_path);
 			goto no_snapshot;
 		}
+		talloc_free(snapshot_path);
 	}
 
 	if (pstripped != NULL) {
@@ -304,6 +393,11 @@ static char *shadow_copy2_find_mount_point(TALLOC_CTX *mem_ctx,
 	return path;
 }
 
+/**
+ * Convert from a name as handed in via the SMB layer
+ * and a timestamp into the local path of the snapshot
+ * of the provided file at the provided time.
+ */
 static char *shadow_copy2_convert(TALLOC_CTX *mem_ctx,
 				  struct vfs_handle_struct *handle,
 				  const char *name, time_t timestamp)
@@ -319,6 +413,55 @@ static char *shadow_copy2_convert(TALLOC_CTX *mem_ctx,
 	size_t insertlen;
 	int i, saved_errno;
 	size_t min_offset;
+	struct shadow_copy2_config *config;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy2_config,
+				return NULL);
+
+	DEBUG(10, ("converting '%s'\n", name));
+
+	if (!config->snapdirseverywhere) {
+		int ret;
+		char *snapshot_path;
+
+		snapshot_path = shadow_copy2_snapshot_path(talloc_tos(),
+							   handle,
+							   timestamp);
+		if (snapshot_path == NULL) {
+			goto fail;
+		}
+
+		if (config->rel_connectpath == NULL) {
+			converted = talloc_asprintf(mem_ctx, "%s/%s",
+						    snapshot_path, name);
+		} else {
+			converted = talloc_asprintf(mem_ctx, "%s/%s/%s",
+						    snapshot_path,
+						    config->rel_connectpath,
+						    name);
+		}
+		if (converted == NULL) {
+			goto fail;
+		}
+
+		ZERO_STRUCT(converted_fname);
+		converted_fname.base_name = converted;
+
+		ret = SMB_VFS_NEXT_LSTAT(handle, &converted_fname);
+		DEBUG(10, ("Trying[not snapdirseverywhere] %s: %d (%s)\n",
+			   converted,
+			   ret, ret == 0 ? "ok" : strerror(errno)));
+		if (ret == 0) {
+			DEBUG(10, ("Found %s\n", converted));
+			result = converted;
+			converted = NULL;
+			goto fail;
+		} else {
+			errno = ENOENT;
+			goto fail;
+		}
+		/* never reached ... */
+	}
 
 	path = talloc_asprintf(mem_ctx, "%s/%s", handle->conn->connectpath,
 			       name);
@@ -328,17 +471,26 @@ static char *shadow_copy2_convert(TALLOC_CTX *mem_ctx,
 	}
 	pathlen = talloc_get_size(path)-1;
 
-	DEBUG(10, ("converting %s\n", path));
-
 	if (!shadow_copy2_find_slashes(talloc_tos(), path,
 				       &slashes, &num_slashes)) {
 		goto fail;
 	}
+
 	insert = shadow_copy2_insert_string(talloc_tos(), handle, timestamp);
 	if (insert == NULL) {
 		goto fail;
 	}
 	insertlen = talloc_get_size(insert)-1;
+
+	/*
+	 * Note: We deliberatly don't expensively initialize the
+	 * array with talloc_zero here: Putting zero into
+	 * converted[pathlen+insertlen] below is sufficient, because
+	 * in the following for loop, the insert string is inserted
+	 * at various slash places. So the memory up to position
+	 * pathlen+insertlen will always be initialized when the
+	 * converted string is used.
+	 */
 	converted = talloc_array(mem_ctx, char, pathlen + insertlen + 1);
 	if (converted == NULL) {
 		goto fail;
@@ -361,17 +513,8 @@ static char *shadow_copy2_convert(TALLOC_CTX *mem_ctx,
 
 	min_offset = 0;
 
-	if (!lp_parm_bool(SNUM(handle->conn), "shadow", "crossmountpoints",
-			  false)) {
-		char *mount_point;
-
-		mount_point = shadow_copy2_find_mount_point(talloc_tos(),
-							    handle);
-		if (mount_point == NULL) {
-			goto fail;
-		}
-		min_offset = strlen(mount_point);
-		TALLOC_FREE(mount_point);
+	if (!config->crossmountpoints) {
+		min_offset = strlen(config->mount_point);
 	}
 
 	memcpy(converted, path, pathlen+1);
@@ -399,7 +542,8 @@ static char *shadow_copy2_convert(TALLOC_CTX *mem_ctx,
 
 		ret = SMB_VFS_NEXT_LSTAT(handle, &converted_fname);
 
-		DEBUG(10, ("Trying %s: %d (%s)\n", converted,
+		DEBUG(10, ("Trying[snapdirseverywhere] %s: %d (%s)\n",
+			   converted,
 			   ret, ret == 0 ? "ok" : strerror(errno)));
 		if (ret == 0) {
 			/* success */
@@ -446,7 +590,12 @@ fail:
 static void convert_sbuf(vfs_handle_struct *handle, const char *fname,
 			 SMB_STRUCT_STAT *sbuf)
 {
-	if (lp_parm_bool(SNUM(handle->conn), "shadow", "fixinodes", False)) {
+	struct shadow_copy2_config *config;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy2_config,
+				return);
+
+	if (config->fixinodes) {
 		/* some snapshot systems, like GPFS, return the name
 		   device:inode for the snapshot files as the current
 		   files. That breaks the 'restore' button in the shadow copy
@@ -958,17 +1107,25 @@ done:
 	return result;
 }
 
+/**
+ * Check whether a given directory contains a
+ * snapshot directory as direct subdirectory.
+ * If yes, return the path of the snapshot-subdir,
+ * otherwise return NULL.
+ */
 static char *have_snapdir(struct vfs_handle_struct *handle,
 			  const char *path)
 {
 	struct smb_filename smb_fname;
 	int ret;
+	struct shadow_copy2_config *config;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy2_config,
+				return NULL);
 
 	ZERO_STRUCT(smb_fname);
-	smb_fname.base_name = talloc_asprintf(
-		talloc_tos(), "%s/%s", path,
-		lp_parm_const_string(SNUM(handle->conn), "shadow", "snapdir",
-				     ".snapshots"));
+	smb_fname.base_name = talloc_asprintf(talloc_tos(), "%s/%s",
+					      path, config->snapdir);
 	if (smb_fname.base_name == NULL) {
 		return NULL;
 	}
@@ -981,12 +1138,27 @@ static char *have_snapdir(struct vfs_handle_struct *handle,
 	return NULL;
 }
 
-static char *shadow_copy2_find_snapdir(TALLOC_CTX *mem_ctx,
-				       struct vfs_handle_struct *handle,
-				       struct smb_filename *smb_fname)
+/**
+ * Find the snapshot directory (if any) for the given
+ * filename (which is relative to the share).
+ */
+static const char *shadow_copy2_find_snapdir(TALLOC_CTX *mem_ctx,
+					     struct vfs_handle_struct *handle,
+					     struct smb_filename *smb_fname)
 {
 	char *path, *p;
-	char *snapdir;
+	const char *snapdir;
+	struct shadow_copy2_config *config;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy2_config,
+				return NULL);
+
+	/*
+	 * If the non-snapdisrseverywhere mode, we should not search!
+	 */
+	if (!config->snapdirseverywhere) {
+		return config->snapshot_basepath;
+	}
 
 	path = talloc_asprintf(mem_ctx, "%s/%s",
 			       handle->conn->connectpath,
@@ -1023,14 +1195,18 @@ static bool shadow_copy2_snapshot_to_gmt(vfs_handle_struct *handle,
 	time_t timestamp_t;
 	unsigned long int timestamp_long;
 	const char *fmt;
+	struct shadow_copy2_config *config;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy2_config,
+				return NULL);
 
-	fmt = lp_parm_const_string(SNUM(handle->conn), "shadow",
-				   "format", GMT_FORMAT);
+	fmt = config->gmt_format;
 
 	ZERO_STRUCT(timestamp);
-	if (lp_parm_bool(SNUM(handle->conn), "shadow", "sscanf", false)) {
+	if (config->use_sscanf) {
 		if (sscanf(name, fmt, &timestamp_long) != 1) {
-			DEBUG(10, ("shadow_copy2_snapshot_to_gmt: no sscanf match %s: %s\n",
+			DEBUG(10, ("shadow_copy2_snapshot_to_gmt: "
+				   "no sscanf match %s: %s\n",
 				   fmt, name));
 			return false;
 		}
@@ -1038,13 +1214,15 @@ static bool shadow_copy2_snapshot_to_gmt(vfs_handle_struct *handle,
 		gmtime_r(&timestamp_t, &timestamp);
 	} else {
 		if (strptime(name, fmt, &timestamp) == NULL) {
-			DEBUG(10, ("shadow_copy2_snapshot_to_gmt: no match %s: %s\n",
+			DEBUG(10, ("shadow_copy2_snapshot_to_gmt: "
+				   "no match %s: %s\n",
 				   fmt, name));
 			return false;
 		}
-		DEBUG(10, ("shadow_copy2_snapshot_to_gmt: match %s: %s\n", fmt, name));
+		DEBUG(10, ("shadow_copy2_snapshot_to_gmt: match %s: %s\n",
+			   fmt, name));
 		
-		if (lp_parm_bool(SNUM(handle->conn), "shadow", "localtime", false)) {
+		if (config->use_localtime) {
 			timestamp.tm_isdst = -1;
 			timestamp_t = mktime(&timestamp);
 			gmtime_r(&timestamp_t, &timestamp);
@@ -1073,9 +1251,12 @@ static void shadow_copy2_sort_data(vfs_handle_struct *handle,
 {
 	int (*cmpfunc)(const void *, const void *);
 	const char *sort;
+	struct shadow_copy2_config *config;
 
-	sort = lp_parm_const_string(SNUM(handle->conn), "shadow",
-				    "sort", "desc");
+	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy2_config,
+				return);
+
+	sort = config->sort_order;
 	if (sort == NULL) {
 		return;
 	}
@@ -1508,9 +1689,281 @@ static int shadow_copy2_get_real_filename(struct vfs_handle_struct *handle,
 	return ret;
 }
 
+static uint64_t shadow_copy2_disk_free(vfs_handle_struct *handle,
+				       const char *path, bool small_query,
+				       uint64_t *bsize, uint64_t *dfree,
+				       uint64_t *dsize)
+{
+	time_t timestamp;
+	char *stripped;
+	ssize_t ret;
+	int saved_errno;
+	char *conv;
+
+	if (!shadow_copy2_strip_snapshot(talloc_tos(), handle, path,
+					 &timestamp, &stripped)) {
+		return -1;
+	}
+	if (timestamp == 0) {
+		return SMB_VFS_NEXT_DISK_FREE(handle, path, small_query,
+					      bsize, dfree, dsize);
+	}
+
+	conv = shadow_copy2_convert(talloc_tos(), handle, stripped, timestamp);
+	TALLOC_FREE(stripped);
+	if (conv == NULL) {
+		return -1;
+	}
+
+	ret = SMB_VFS_NEXT_DISK_FREE(handle, conv, small_query, bsize, dfree,
+				     dsize);
+
+	saved_errno = errno;
+	TALLOC_FREE(conv);
+	errno = saved_errno;
+
+	return ret;
+}
+
+static int shadow_copy2_connect(struct vfs_handle_struct *handle,
+				const char *service, const char *user)
+{
+	struct shadow_copy2_config *config;
+	int ret;
+	const char *snapdir;
+	const char *gmt_format;
+	const char *sort_order;
+	const char *basedir;
+	const char *mount_point;
+
+	DEBUG(10, (__location__ ": cnum[%u], connectpath[%s]\n",
+		   (unsigned)handle->conn->cnum,
+		   handle->conn->connectpath));
+
+	ret = SMB_VFS_NEXT_CONNECT(handle, service, user);
+	if (ret < 0) {
+		return ret;
+	}
+
+	config = talloc_zero(handle->conn, struct shadow_copy2_config);
+	if (config == NULL) {
+		DEBUG(0, ("talloc_zero() failed\n"));
+		errno = ENOMEM;
+		return -1;
+	}
+
+	gmt_format = lp_parm_const_string(SNUM(handle->conn),
+					  "shadow", "format",
+					  GMT_FORMAT);
+	config->gmt_format = talloc_strdup(config, gmt_format);
+	if (config->gmt_format == NULL) {
+		DEBUG(0, ("talloc_strdup() failed\n"));
+		errno = ENOMEM;
+		return -1;
+	}
+
+	config->use_sscanf = lp_parm_bool(SNUM(handle->conn),
+					  "shadow", "sscanf", false);
+
+	config->use_localtime = lp_parm_bool(SNUM(handle->conn),
+					     "shadow", "localtime",
+					     false);
+
+	snapdir = lp_parm_const_string(SNUM(handle->conn),
+				       "shadow", "snapdir",
+				       ".snapshots");
+	config->snapdir = talloc_strdup(config, snapdir);
+	if (config->snapdir == NULL) {
+		DEBUG(0, ("talloc_strdup() failed\n"));
+		errno = ENOMEM;
+		return -1;
+	}
+
+	config->snapdirseverywhere = lp_parm_bool(SNUM(handle->conn),
+						  "shadow",
+						  "snapdirseverywhere",
+						  false);
+
+	config->crossmountpoints = lp_parm_bool(SNUM(handle->conn),
+						"shadow", "crossmountpoints",
+						false);
+
+	config->fixinodes = lp_parm_bool(SNUM(handle->conn),
+					 "shadow", "fixinodes",
+					 false);
+
+	sort_order = lp_parm_const_string(SNUM(handle->conn),
+					  "shadow", "sort", "desc");
+	config->sort_order = talloc_strdup(config, sort_order);
+	if (config->sort_order == NULL) {
+		DEBUG(0, ("talloc_strdup() failed\n"));
+		errno = ENOMEM;
+		return -1;
+	}
+
+	mount_point = lp_parm_const_string(SNUM(handle->conn),
+					   "shadow", "mountpoint", NULL);
+	if (mount_point != NULL) {
+		if (mount_point[0] != '/') {
+			DEBUG(1, (__location__ " Warning: 'mountpoint' is "
+				  "relative ('%s'), but it has to be an "
+				  "absolute path. Ignoring provided value.\n",
+				  mount_point));
+			mount_point = NULL;
+		} else {
+			char *p;
+			p = strstr(handle->conn->connectpath, mount_point);
+			if (p != handle->conn->connectpath) {
+				DEBUG(1, ("Warning: mount_point (%s) is not a "
+					  "subdirectory of the share root "
+					  "(%s). Ignoring provided value.\n",
+					  mount_point,
+					  handle->conn->connectpath));
+				mount_point = NULL;
+			}
+		}
+	}
+
+	if (mount_point != NULL) {
+		config->mount_point = talloc_strdup(config, mount_point);
+		if (config->mount_point == NULL) {
+			DEBUG(0, (__location__ " talloc_strdup() failed\n"));
+			return -1;
+		}
+	} else {
+		config->mount_point = shadow_copy2_find_mount_point(config,
+								    handle);
+		if (config->mount_point == NULL) {
+			DEBUG(0, (__location__ ": shadow_copy2_find_mount_point"
+				  " failed: %s\n", strerror(errno)));
+			return -1;
+		}
+	}
+
+	basedir = lp_parm_const_string(SNUM(handle->conn),
+				       "shadow", "basedir", NULL);
+
+	if (basedir != NULL) {
+		if (basedir[0] != '/') {
+			DEBUG(1, (__location__ " Warning: 'basedir' is "
+				  "relative ('%s'), but it has to be an "
+				  "absolute path. Disabling basedir.\n",
+				  basedir));
+		} else {
+			char *p;
+			p = strstr(basedir, config->mount_point);
+			if (p != basedir) {
+				DEBUG(1, ("Warning: basedir (%s) is not a "
+					  "subdirectory of the share root's "
+					  "mount point (%s). "
+					  "Disabling basedir\n",
+					  basedir, config->mount_point));
+			} else {
+				config->basedir = talloc_strdup(config,
+								basedir);
+				if (config->basedir == NULL) {
+					DEBUG(0, ("talloc_strdup() failed\n"));
+					errno = ENOMEM;
+					return -1;
+				}
+			}
+		}
+	}
+
+	if (config->snapdirseverywhere && config->basedir != NULL) {
+		DEBUG(1, (__location__ " Warning: 'basedir' is incompatible "
+			  "with 'snapdirseverywhere'. Disabling basedir.\n"));
+		TALLOC_FREE(config->basedir);
+	}
+
+	if (config->crossmountpoints && config->basedir != NULL) {
+		DEBUG(1, (__location__ " Warning: 'basedir' is incompatible "
+			  "with 'crossmountpoints'. Disabling basedir.\n"));
+		TALLOC_FREE(config->basedir);
+	}
+
+	if (config->basedir == NULL) {
+		config->basedir = config->mount_point;
+	}
+
+	if (strlen(config->basedir) != strlen(handle->conn->connectpath)) {
+		config->rel_connectpath = talloc_strdup(config,
+			handle->conn->connectpath + strlen(config->basedir));
+		if (config->rel_connectpath == NULL) {
+			DEBUG(0, ("talloc_strdup() failed\n"));
+			errno = ENOMEM;
+			return -1;
+		}
+	}
+
+	if (config->snapdir[0] == '/') {
+		config->snapdir_absolute = true;
+
+		if (config->snapdirseverywhere == true) {
+			DEBUG(1, (__location__ " Warning: An absolute snapdir "
+				  "is incompatible with 'snapdirseverywhere', "
+				  "setting 'snapdirseverywhere' to false.\n"));
+			config->snapdirseverywhere = false;
+		}
+
+		if (config->crossmountpoints == true) {
+			DEBUG(1, (__location__ " Warning: 'crossmountpoints' "
+				  "is not supported with an absolute snapdir. "
+				  "Disabling it.\n"));
+			config->crossmountpoints = false;
+		}
+
+		config->snapshot_basepath = config->snapdir;
+	} else {
+		config->snapshot_basepath = talloc_asprintf(config, "%s/%s",
+				config->mount_point, config->snapdir);
+		if (config->snapshot_basepath == NULL) {
+			DEBUG(0, ("talloc_asprintf() failed\n"));
+			errno = ENOMEM;
+			return -1;
+		}
+	}
+
+	DEBUG(10, ("shadow_copy2_connect: configuration:\n"
+		   "  share root: '%s'\n"
+		   "  basedir: '%s'\n"
+		   "  mountpoint: '%s'\n"
+		   "  rel share root: '%s'\n"
+		   "  snapdir: '%s'\n"
+		   "  snapshot base path: '%s'\n"
+		   "  format: '%s'\n"
+		   "  use sscanf: %s\n"
+		   "  snapdirs everywhere: %s\n"
+		   "  cross mountpoints: %s\n"
+		   "  fix inodes: %s\n"
+		   "  sort order: %s\n"
+		   "",
+		   handle->conn->connectpath,
+		   config->basedir,
+		   config->mount_point,
+		   config->rel_connectpath,
+		   config->snapdir,
+		   config->snapshot_basepath,
+		   config->gmt_format,
+		   config->use_sscanf ? "yes" : "no",
+		   config->snapdirseverywhere ? "yes" : "no",
+		   config->crossmountpoints ? "yes" : "no",
+		   config->fixinodes ? "yes" : "no",
+		   config->sort_order
+		   ));
+
+
+	SMB_VFS_HANDLE_SET_DATA(handle, config,
+				NULL, struct shadow_copy2_config,
+				return -1);
+
+	return 0;
+}
 
 static struct vfs_fn_pointers vfs_shadow_copy2_fns = {
+	.connect_fn = shadow_copy2_connect,
 	.opendir_fn = shadow_copy2_opendir,
+	.disk_free_fn = shadow_copy2_disk_free,
 	.rename_fn = shadow_copy2_rename,
 	.link_fn = shadow_copy2_link,
 	.symlink_fn = shadow_copy2_symlink,
