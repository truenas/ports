--- source3/modules/vfs_shadow_copy_zfs.c.orig	2018-12-14 14:59:05.840498000 -0500
+++ source3/modules/vfs_shadow_copy_zfs.c	2018-12-14 15:19:52.068322000 -0500
@@ -0,0 +1,1444 @@
+/* shadow_copy_zfs: a shadow copy module for ZFS
+ *
+ * Copyright (C) Andrew Tridgell   2007 (portions taken from shadow_copy_zfs)
+ * Copyright (C) Ed Plese          2009
+ * Copyright (C) Volker Lendecke   2011
+ * Copyright (C) Christian Ambach  2011
+ * Copyright (C) Michael Adam      2013
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
+#include "includes.h"
+#include "smbd/smbd.h"
+#include "system/filesys.h"
+#include "include/ntioctl.h"
+#include "zfs_list_snapshots.h"
+
+#define GMT_NAME_LEN 24 /* length of a @GMT- name */
+
+#define SHADOW_COPY_ZFS_DEFAULT_SORT "desc"
+#define SHADOW_COPY_ZFS_SNAP_DIR ".zfs/snapshot"
+#define MAX_CACHE_TIME 10.0
+
+static const char *null_string = NULL;
+static const char **empty_list = &null_string;
+
+struct shadow_copy_zfs_config {
+    const char *sort_order;
+    const char *dataset_path;
+    const char **inclusions;
+    const char **exclusions;
+    struct snapshot_list *snapshots;
+};
+
+static bool shadow_copy_zfs_find_slashes(TALLOC_CTX *mem_ctx, const char *str,
+				      size_t **poffsets,
+				      unsigned *pnum_offsets)
+{
+	unsigned num_offsets;
+	size_t *offsets;
+	const char *p;
+
+	num_offsets = 0;
+
+	p = str;
+	while ((p = strchr(p, '/')) != NULL) {
+		num_offsets += 1;
+		p += 1;
+	}
+
+	offsets = talloc_array(mem_ctx, size_t, num_offsets);
+	if (offsets == NULL) {
+		return false;
+	}
+
+	p = str;
+	num_offsets = 0;
+	while ((p = strchr(p, '/')) != NULL) {
+		offsets[num_offsets] = p-str;
+		num_offsets += 1;
+		p += 1;
+	}
+
+	*poffsets = offsets;
+	*pnum_offsets = num_offsets;
+	return true;
+}
+
+/*
+  make very sure it is one of our special names
+ */
+static inline bool shadow_copy_zfs_match_name(const char *name,
+    const char **gmt_start)
+{
+	unsigned year, month, day, hr, min, sec;
+	const char *p;
+	if (gmt_start) {
+		(*gmt_start) = NULL;
+	}
+	p = strstr_m(name, "@GMT-");
+	if (p == NULL) return false;
+	if (p > name && p[-1] != '/') return False;
+	if (sscanf(p, "@GMT-%04u.%02u.%02u-%02u.%02u.%02u", &year, &month,
+		   &day, &hr, &min, &sec) != 6) {
+		return False;
+	}
+	if (p[24] != 0 && p[24] != '/') {
+		return False;
+	}
+	if (gmt_start) {
+		(*gmt_start) = p;
+	}
+	return True;
+}
+
+/*
+  shadow copy paths can also come into the server in this form:
+
+    /foo/bar/@GMT-XXXXX/some/file
+
+  This function normalises the filename to be of the form:
+
+    @GMT-XXXX/foo/bar/some/file
+ */
+static const char *shadow_copy_zfs_normalise_path(TALLOC_CTX *mem_ctx,
+    const char *path,
+    const char *gmt_start)
+{
+	char *pcopy;
+	char buf[GMT_NAME_LEN];
+	size_t prefix_len;
+
+	if (path == gmt_start) {
+		return path;
+	}
+
+	prefix_len = gmt_start - path - 1;
+
+	DEBUG(10, ("path=%s, gmt_start=%s, prefix_len=%d\n", path, gmt_start,
+	    (int)prefix_len));
+
+	/*
+	 * We've got a/b/c/@GMT-YYYY.MM.DD-HH.MM.SS/d/e. convert to
+	 * @GMT-YYYY.MM.DD-HH.MM.SS/a/b/c/d/e before further
+	 * processing. As many VFS calls provide a const char *,
+	 * unfortunately we have to make a copy.
+	 */
+
+	pcopy = talloc_strdup(mem_ctx, path);
+	if (pcopy == NULL) {
+		return NULL;
+	}
+
+	gmt_start = pcopy + prefix_len;
+
+	/*
+	 * Copy away "@GMT-YYYY.MM.DD-HH.MM.SS"
+	 */
+	memcpy(buf, gmt_start+1, GMT_NAME_LEN);
+
+	/*
+	 * Make space for it including a trailing /
+	 */
+	memmove(pcopy + GMT_NAME_LEN + 1, pcopy, prefix_len);
+
+	/*
+	 * Move in "@GMT-YYYY.MM.DD-HH.MM.SS/" at the beginning again
+	 */
+	memcpy(pcopy, buf, GMT_NAME_LEN);
+	pcopy[GMT_NAME_LEN] = '/';
+
+	DEBUG(10, ("shadow_copy_zfs_normalise_path: %s -> %s\n", path, pcopy));
+
+	return pcopy;
+}
+
+static int shadow_copy_zfs_label_cmp_asc(const void *x, const void *y)
+{
+	return strncmp((*((struct snapshot_entry **) x))->label,
+		       (*((struct snapshot_entry **) y))->label,
+		       sizeof(SHADOW_COPY_LABEL));
+}
+
+static int shadow_copy_zfs_label_cmp_desc(const void *x, const void *y)
+{
+	return -strncmp((*((struct snapshot_entry **) x))->label,
+			(*((struct snapshot_entry **) y))->label,
+			sizeof(SHADOW_COPY_LABEL));
+}
+
+/*
+  sort the shadow copy data in ascending or descending order
+ */
+static void shadow_copy_zfs_sort_data(vfs_handle_struct *handle,
+    struct snapshot_list *snapshots)
+{
+	const char *sort;
+
+	if (snapshots->num_entries <= 0) {
+		return;
+	}
+
+	sort = lp_parm_const_string(SNUM(handle->conn), "shadow",
+				    "sort", SHADOW_COPY_ZFS_DEFAULT_SORT);
+
+	if (strcmp(sort, "asc") == 0) {
+		snapshots->cmpfunc = shadow_copy_zfs_label_cmp_asc;
+	} else {
+		snapshots->cmpfunc = shadow_copy_zfs_label_cmp_desc;
+	}
+
+	TYPESAFE_QSORT(snapshots->entries, snapshots->num_entries,
+		       snapshots->cmpfunc);
+
+	return;
+}
+
+static void shadow_copy_zfs_free_snapshots(void **datap)
+{
+	TALLOC_FREE(*datap);
+}
+
+/*
+  convert a filename from a share relative path, to a path in the
+  snapshot directory
+ */
+static char *convert_shadow_zfs_name(vfs_handle_struct *handle,
+    const char *fname, const char *gmt_path,
+    const bool incl_rel)
+{
+	TALLOC_CTX *tmp_ctx = talloc_new(handle->data);
+	struct shadow_copy_zfs_config *config;
+	struct snapshot_list *snapshots;
+	struct snapshot_entry entry_buf, *entry = &entry_buf;
+	const char *relpath, *mpoffset, *mountpoint, *snapshot;
+	size_t mplen;
+	char *ret, *prefix;
+	unsigned idx;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy_zfs_config,
+	    return NULL);
+
+	/* get the snapshot info */
+	snapshots = shadow_copy_zfs_list_snapshots(tmp_ctx,
+						   config->dataset_path,
+						   config->inclusions,
+						   config->exclusions);
+
+	if (snapshots == NULL) {
+		talloc_free(tmp_ctx);
+		return NULL;
+	}
+
+	shadow_copy_zfs_sort_data(handle, snapshots);
+
+	/* get the mountpoint */
+	mountpoint = snapshots->mountpoint;
+	mplen = strlen(mountpoint);
+	mpoffset = handle->conn->connectpath + mplen;
+
+	/* some sanity checks */
+	if (strncmp(mountpoint, handle->conn->connectpath, mplen) != 0 ||
+	    (handle->conn->connectpath[mplen] != 0 &&
+	     handle->conn->connectpath[mplen] != '/')) {
+		DEBUG(0,("convert_shadow_zfs_name: mountpoint %s is not a "
+		    "parent of %s\n", mountpoint,
+		    handle->conn->connectpath));
+		talloc_free(tmp_ctx);
+		return NULL;
+	}
+
+	/* check if we've already normalized this */
+	prefix = talloc_asprintf(tmp_ctx, "%s/%s/", mountpoint,
+				 SHADOW_COPY_ZFS_SNAP_DIR);
+	if (strncmp(fname, prefix, (talloc_get_size(prefix)-1)) == 0) {
+		/* this looks like as we have already normalized it, leave it
+		   untouched
+		*/
+		talloc_free(tmp_ctx);
+		return talloc_strdup(handle->data, fname);
+	}
+
+	if (strncmp(fname, "@GMT-", 5) != 0) {
+		fname = shadow_copy_zfs_normalise_path(tmp_ctx, fname, gmt_path);
+		if (fname == NULL) {
+			talloc_free(tmp_ctx);
+			return NULL;
+		}
+	}
+
+	/* get snapshot name */
+	strlcpy(entry->label, fname, GMT_NAME_LEN+1);
+	snapshot = bsearch(&entry, snapshots->entries, snapshots->num_entries,
+			   sizeof(snapshots->entries[0]), snapshots->cmpfunc);
+	for (idx = 0; idx < snapshots->num_entries; idx++) {
+		if (strncmp(fname, snapshots->entries[idx]->label, GMT_NAME_LEN)
+		    == 0) {
+			snapshot = snapshots->entries[idx]->name;
+			break;
+		}
+	}
+
+	if (snapshot == NULL) {
+		DEBUG(1,("convert_shadow_zfs_name: no snapshot found for %s\n",
+		    fname));
+		talloc_free(tmp_ctx);
+		return NULL;
+	}
+
+	/* assemble the new path */
+	relpath = fname + GMT_NAME_LEN;
+
+	if (*relpath == '/') relpath++;
+	if (*mpoffset == '/') mpoffset++;
+
+	ret = talloc_asprintf(handle->data, "%s/%s/%s%s%s%s%s",
+			      mountpoint,
+			      SHADOW_COPY_ZFS_SNAP_DIR,
+			      snapshot,
+			      *mpoffset ? "/" : "",
+			      mpoffset,
+			      *relpath ? "/" : "",
+			      incl_rel ? relpath : "");
+	DEBUG(6,("convert_shadow_zfs_name: '%s' -> '%s'\n", fname, ret));
+
+	talloc_free(tmp_ctx);
+	return ret;
+}
+
+static DIR *shadow_copy_zfs_opendir(vfs_handle_struct *handle,
+    const struct smb_filename *smb_fname,
+    const char *mask,
+    uint32_t attr)
+{
+	DIR *ret;
+	int saved_errno;
+	const char *gmt_start;
+	char *conv;
+	struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name,
+		     gmt_start, True);
+		if (conv == NULL) {
+			return NULL;
+		}
+
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						     conv,
+						     NULL,
+						     NULL,
+						     smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return NULL;
+		}
+
+		ret = SMB_VFS_NEXT_OPENDIR(handle, conv_smb_fname, mask, attr);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_OPENDIR(handle, smb_fname, mask, attr);
+	}
+}
+
+static int shadow_copy_zfs_rename(vfs_handle_struct *handle,
+			       const struct smb_filename *smb_fname_src,
+			       const struct smb_filename *smb_fname_dst)
+{
+	const char *gmt_start;
+	int ret_src, ret_dst;
+
+	ret_src = shadow_copy_zfs_match_name(smb_fname_src->base_name,
+					     &gmt_start);
+	ret_dst = shadow_copy_zfs_match_name(smb_fname_dst->base_name,
+					     &gmt_start);
+
+	if (ret_src != 0) {
+		errno = EXDEV;
+		return -1;
+	}
+
+	if (ret_dst != 0) {
+		errno = EROFS;
+		return -1;
+	}
+
+	return SMB_VFS_NEXT_RENAME(handle, smb_fname_src, smb_fname_dst);
+}
+
+static int shadow_copy_zfs_symlink(vfs_handle_struct *handle,
+				const char *link_contents, const struct smb_filename *new_smb_filename)
+{
+	const char *gmt_start;
+	int ret_old, ret_new;
+
+	ret_old = shadow_copy_zfs_match_name(link_contents, &gmt_start);
+	ret_new = shadow_copy_zfs_match_name(new_smb_filename->base_name, &gmt_start);
+
+	if ((ret_old != 0) || (ret_new != 0)) {
+		errno = EROFS;
+		return -1;
+	}
+
+	return SMB_VFS_NEXT_SYMLINK(handle, link_contents, new_smb_filename);
+}
+
+static int shadow_copy_zfs_link(vfs_handle_struct *handle,
+			     const struct smb_filename *oldname, 
+			     const struct smb_filename *newname)
+{
+	const char *gmt_start;
+	int ret_old, ret_new;
+
+	ret_old = shadow_copy_zfs_match_name(oldname->base_name, &gmt_start);
+	ret_new = shadow_copy_zfs_match_name(newname->base_name, &gmt_start);
+
+	if ((ret_old != 0) || (ret_new != 0)) {
+		errno = EROFS;
+		return -1;
+	}
+
+	return SMB_VFS_NEXT_LINK(handle, oldname, newname);
+}
+
+static int shadow_copy_zfs_stat(vfs_handle_struct *handle,
+			     struct smb_filename *smb_fname)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *tmp;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		tmp = smb_fname->base_name;
+		smb_fname->base_name = convert_shadow_zfs_name(
+		    handle, smb_fname->base_name, gmt_start, True);
+
+		if (smb_fname->base_name == NULL) {
+			smb_fname->base_name = tmp;
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_STAT(handle, smb_fname);
+		saved_errno = errno;
+
+		TALLOC_FREE(smb_fname->base_name);
+		smb_fname->base_name = tmp;
+
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_STAT(handle, smb_fname);
+	}
+}
+
+static int shadow_copy_zfs_lstat(vfs_handle_struct *handle,
+			      struct smb_filename *smb_fname)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *tmp;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		tmp = smb_fname->base_name;
+		smb_fname->base_name = convert_shadow_zfs_name(
+		    handle, smb_fname->base_name, gmt_start, True);
+
+		if (smb_fname->base_name == NULL) {
+			smb_fname->base_name = tmp;
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_LSTAT(handle, smb_fname);
+		saved_errno = errno;
+
+		TALLOC_FREE(smb_fname->base_name);
+		smb_fname->base_name = tmp;
+
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_LSTAT(handle, smb_fname);
+	}
+}
+
+static int shadow_copy_zfs_fstat(vfs_handle_struct *handle, files_struct *fsp,
+			      SMB_STRUCT_STAT *sbuf)
+{
+	int ret;
+	const char *gmt_start;
+
+	ret = SMB_VFS_NEXT_FSTAT(handle, fsp, sbuf);
+	if (ret == -1) {
+		return ret;
+	}
+
+	return 0;
+}
+
+static int shadow_copy_zfs_open(vfs_handle_struct *handle,
+			     struct smb_filename *smb_fname, files_struct *fsp,
+			     int flags, mode_t mode)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *tmp;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		tmp = smb_fname->base_name;
+		smb_fname->base_name = convert_shadow_zfs_name(
+		    handle, smb_fname->base_name, gmt_start, True);
+
+		if (smb_fname->base_name == NULL) {
+			smb_fname->base_name = tmp;
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_OPEN(handle, smb_fname, fsp, flags, mode);
+		saved_errno = errno;
+
+		TALLOC_FREE(smb_fname->base_name);
+		smb_fname->base_name = tmp;
+
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_OPEN(handle, smb_fname, fsp, flags, mode);
+	}
+}
+
+static int shadow_copy_zfs_unlink(vfs_handle_struct *handle,
+			       const struct smb_filename *smb_fname) {
+	int ret, saved_errno;
+	const char *gmt_start;
+	struct smb_filename *conv;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = cp_smb_filename(talloc_tos(), smb_fname);
+		if (conv == NULL) {
+			errno = ENOMEM;
+			return -1;
+		}
+
+		conv->base_name = convert_shadow_zfs_name(handle,
+							  smb_fname->base_name,
+							  gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_UNLINK(handle, conv);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_UNLINK(handle, smb_fname);
+	}
+}
+
+static int shadow_copy_zfs_chmod(vfs_handle_struct *handle,
+    const struct smb_filename *smb_fname,
+    mode_t mode)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *conv;
+	struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name,
+					       gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						     conv,
+						     NULL,
+						     NULL,
+						     smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			errno = ENOMEM;
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_CHMOD(handle, conv_smb_fname, mode);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_CHMOD(handle, smb_fname, mode);
+	}
+}
+
+static int shadow_copy_zfs_chown(vfs_handle_struct *handle,
+    const struct smb_filename *smb_fname,
+    uid_t uid,
+    gid_t gid)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *conv;
+	struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name,
+					       gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						     conv,
+						     NULL,
+						     NULL,
+						     smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			errno = ENOMEM;
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_CHOWN(handle, conv_smb_fname, uid, gid);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_CHOWN(handle, smb_fname, uid, gid);
+	}
+}
+
+static int shadow_copy_zfs_chdir(vfs_handle_struct *handle,
+			      const struct smb_filename *smb_fname)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *conv;
+	struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name, gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						     conv,
+						     NULL,
+						     NULL,
+						     smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_CHDIR(handle, conv_smb_fname);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_CHDIR(handle, smb_fname);
+	}
+}
+
+static int shadow_copy_zfs_ntimes(vfs_handle_struct *handle,
+			       const struct smb_filename *smb_fname,
+			       struct smb_file_time *ft) {
+	int ret, saved_errno;
+	const char *gmt_start;
+	struct smb_filename *conv;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = cp_smb_filename(talloc_tos(), smb_fname);
+		if (conv == NULL) {
+			errno = ENOMEM;
+			return -1;
+		}
+
+		conv->base_name = convert_shadow_zfs_name(handle,
+							  smb_fname->base_name,
+							  gmt_start, True);
+		if (conv->base_name == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_NTIMES(handle, conv, ft);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_NTIMES(handle, smb_fname, ft);
+	}
+}
+
+static int shadow_copy_zfs_readlink(vfs_handle_struct *handle,
+				 const struct smb_filename *smb_fname, 
+				 char *buf, 
+				 size_t bufsiz)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+ 	struct smb_filename *conv = NULL;	
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = cp_smb_filename(talloc_tos(), smb_fname);
+		if (conv == NULL) {
+			return -1;
+		}
+		conv->base_name = convert_shadow_zfs_name(handle, smb_fname->base_name, gmt_start, True);
+
+		if (conv->base_name == NULL){
+			TALLOC_FREE(conv);
+			return -1;
+		}
+		ret = SMB_VFS_NEXT_READLINK(handle, conv, buf, bufsiz);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_READLINK(handle, smb_fname, buf, bufsiz);
+	}
+}
+
+static int shadow_copy_zfs_mknod(vfs_handle_struct *handle,
+				const struct smb_filename *smb_fname, 
+				mode_t mode, 
+				SMB_DEV_T dev) 
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *conv;
+	const struct smb_filename *conv_smb_fname;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name, gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						     conv,
+						     NULL,
+						     NULL,
+						     smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_MKNOD(handle, conv_smb_fname, mode, dev);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_MKNOD(handle, smb_fname, mode, dev);
+	}
+}
+
+static char *shadow_copy_zfs_realpath(vfs_handle_struct *handle, TALLOC_CTX *ctx,
+				   const struct smb_filename *smb_fname)
+{
+	struct smb_filename *ret = NULL;
+	int saved_errno;
+	const char *gmt_start;
+	char *conv;
+	const struct smb_filename *conv_smb_fname;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name, gmt_start, True);
+		if (conv == NULL) {
+			TALLOC_FREE(conv);
+			return NULL;
+		}
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						     conv,
+						     NULL,
+						     NULL,
+						     smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_REALPATH(handle, ctx, conv_smb_fname);
+		DEBUG(1, ("shadow: realpath for %s is %s\n", conv_smb_fname->base_name, ret->base_name));
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_REALPATH(handle, ctx, smb_fname);
+	}
+}
+
+static int shadow_copy_zfs_get_shadow_copy_zfs_data(vfs_handle_struct *handle,
+						    files_struct *fsp,
+						    struct shadow_copy_data
+						    *shadow_copy_zfs_data,
+						    bool labels)
+{
+	TALLOC_CTX *tmp_ctx;
+	struct shadow_copy_zfs_config *config;
+	struct snapshot_list *snapshots;
+	unsigned idx;
+
+	SMB_VFS_HANDLE_GET_DATA(handle, config, struct shadow_copy_zfs_config,
+	    return -1);
+
+	tmp_ctx = talloc_new(config);
+	snapshots = shadow_copy_zfs_list_snapshots(tmp_ctx,
+						   config->dataset_path,
+						   config->inclusions,
+						   config->exclusions);
+
+	if (snapshots == NULL) {
+		talloc_free(tmp_ctx);
+		return -1;
+	}
+
+	shadow_copy_zfs_sort_data(handle, snapshots);
+	shadow_copy_zfs_data->num_volumes = snapshots->num_entries;
+	shadow_copy_zfs_data->labels = NULL;
+
+	if (labels) {
+		shadow_copy_zfs_data->labels =
+			talloc_array(shadow_copy_zfs_data,
+				     SHADOW_COPY_LABEL,
+				     shadow_copy_zfs_data->num_volumes);
+
+		if (shadow_copy_zfs_data->labels == NULL) {
+			DEBUG(0,("shadow_copy_zfs: out of memory\n"));
+			return -1;
+		}
+
+		for (idx = 0; idx < snapshots->num_entries; idx++) {
+			strlcpy(shadow_copy_zfs_data->labels[idx],
+				snapshots->entries[idx]->label,
+				sizeof(shadow_copy_zfs_data->labels[0]));
+		}
+	}
+
+	talloc_free(tmp_ctx);
+	return 0;
+}
+
+static NTSTATUS shadow_copy_zfs_fget_nt_acl(vfs_handle_struct *handle,
+					struct files_struct *fsp,
+					uint32_t security_info,
+					TALLOC_CTX *mem_ctx,
+					struct security_descriptor **ppdesc)
+{
+	NTSTATUS ret;
+	int saved_errno;
+	const char *gmt_start;
+	char *conv;
+	struct smb_filename *smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(fsp->fsp_name->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, fsp->fsp_name->base_name,
+					       gmt_start, True);
+		if (conv == NULL) {
+			return map_nt_error_from_unix(errno);
+		}
+
+		smb_fname = synthetic_smb_fname(talloc_tos(),
+						conv,
+						NULL,
+						NULL,
+						fsp->fsp_name->flags);
+		if (smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return NT_STATUS_NO_MEMORY;
+		}
+
+		ret = SMB_VFS_NEXT_GET_NT_ACL(handle, smb_fname, security_info,
+					      mem_ctx, ppdesc);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_FGET_NT_ACL(handle, fsp, security_info,
+					       mem_ctx, ppdesc);
+	}
+}
+
+static NTSTATUS shadow_copy_zfs_get_nt_acl(vfs_handle_struct *handle,
+    const struct smb_filename *smb_fname,
+    uint32_t security_info,
+    TALLOC_CTX *mem_ctx,
+    struct security_descriptor **ppdesc)
+{
+	NTSTATUS ret;
+	int saved_errno;
+	const char *gmt_start;
+	char *conv;
+	struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name,
+					       gmt_start, True);
+		if (conv == NULL) {
+			return map_nt_error_from_unix(errno);
+		}
+
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						     conv,
+						     NULL,
+						     NULL,
+						     smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return NT_STATUS_NO_MEMORY;
+		}
+
+		ret = SMB_VFS_NEXT_GET_NT_ACL(handle, conv_smb_fname,
+					      security_info, mem_ctx, ppdesc);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_GET_NT_ACL(handle, smb_fname, security_info,
+					       mem_ctx, ppdesc);
+	}
+}
+
+static int shadow_copy_zfs_mkdir(vfs_handle_struct *handle,
+    const struct smb_filename *smb_fname, mode_t mode)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *conv;
+	struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name,
+					       gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						     conv,
+						     NULL,
+						     NULL,
+						     smb_fname->flags);
+
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_MKDIR(handle, conv_smb_fname, mode);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_MKDIR(handle, smb_fname, mode);
+	}
+}
+
+static int shadow_copy_zfs_rmdir(vfs_handle_struct *handle,
+    const struct smb_filename *smb_fname)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *conv;
+	struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name,
+					       gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						     conv,
+						     NULL,
+						     NULL,
+						     smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_RMDIR(handle, conv_smb_fname);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_RMDIR(handle, smb_fname);
+	}
+}
+
+static int shadow_copy_zfs_chflags(vfs_handle_struct *handle, 
+				const struct smb_filename *smb_fname,
+				unsigned int flags)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *conv;
+	const struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name, gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						conv,
+						NULL,
+						NULL,
+						smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_CHFLAGS(handle, conv_smb_fname, flags);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_CHFLAGS(handle, smb_fname, flags);
+	}
+}
+
+static ssize_t shadow_copy_zfs_getxattr(vfs_handle_struct *handle,
+				const struct smb_filename *smb_fname, 
+				const char *aname,
+				void *value, 
+				size_t size)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *conv;
+	const struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name, gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						conv,
+						NULL,
+						NULL,
+						smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_GETXATTR(handle, conv_smb_fname, aname, value,
+					    size);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_GETXATTR(handle, smb_fname, aname, value,
+					     size);
+	}
+}
+
+static ssize_t shadow_copy_zfs_listxattr(struct vfs_handle_struct *handle,
+				const struct smb_filename *smb_fname,
+				char *list, 
+				size_t size)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *conv;
+	const struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name, gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						conv,
+						NULL,
+						NULL,
+						smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_LISTXATTR(handle, conv_smb_fname, list, size);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_LISTXATTR(handle, smb_fname, list, size);
+	}
+}
+
+static int shadow_copy_zfs_removexattr(vfs_handle_struct *handle,
+				const struct smb_filename *smb_fname, 
+				const char *aname)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *conv;
+	const struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name, gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						conv,
+						NULL,
+						NULL,
+						smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_REMOVEXATTR(handle, conv_smb_fname, aname);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_REMOVEXATTR(handle, smb_fname, aname);
+	}
+}
+
+static int shadow_copy_zfs_setxattr(struct vfs_handle_struct *handle,
+				const struct smb_filename *smb_fname,
+				const char *aname, 
+				const void *value,
+				size_t size, 
+				int flags)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *conv;
+	const struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name, gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						conv,
+						NULL,
+						NULL,
+						smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_SETXATTR(handle, conv_smb_fname, aname, value, size,
+					    flags);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_SETXATTR(handle, smb_fname, aname, value, size,
+					     flags);
+	}
+}
+
+static int shadow_copy_zfs_get_real_filename(struct vfs_handle_struct *handle,
+					  const char *path,
+					  const char *name,
+					  TALLOC_CTX *mem_ctx,
+					  char **found_name)
+{
+	const char *gmt_start;
+	ssize_t ret;
+	int saved_errno;
+	char *conv;
+
+
+	if (shadow_copy_zfs_match_name(path, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, path, gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_GET_REAL_FILENAME(handle, conv, name,
+						     mem_ctx, found_name);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_GET_REAL_FILENAME(handle, path, name,
+						      mem_ctx, found_name);
+	}
+}
+
+static const char *shadow_copy_zfs_connectpath(struct vfs_handle_struct *handle,
+					    const struct smb_filename *smb_fname)
+{
+	const char *ret;
+	int saved_errno;
+	const char *gmt_start;
+	char *conv;
+	const struct smb_filename *conv_smb_fname = NULL;
+	const struct smb_filename *tmp_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		TALLOC_CTX *tmp_ctx = talloc_new(handle->data);
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name, gmt_start, True);
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						conv,
+						NULL,
+						NULL,
+						smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			TALLOC_FREE(tmp_ctx);
+			return -1;
+		}
+
+		tmp_smb_fname = SMB_VFS_NEXT_REALPATH(handle, tmp_ctx, conv_smb_fname);
+		saved_errno = errno;
+		TALLOC_FREE(conv_smb_fname);
+		TALLOC_FREE(tmp_ctx);
+		errno = saved_errno;
+		return tmp_smb_fname->base_name;
+	} else {
+		return SMB_VFS_NEXT_CONNECTPATH(handle, smb_fname);
+	}
+}
+
+static uint64_t shadow_copy_zfs_disk_free(vfs_handle_struct *handle,
+				const struct smb_filename *smb_fname, 
+				uint64_t *bsize,
+				uint64_t *dfree, 
+				uint64_t *dsize)
+{
+	uint64_t ret = (uint64_t)-1;
+	int saved_errno;
+	const char *gmt_start;
+	char *conv;
+	struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name, gmt_start, True);
+		if (conv == NULL) {
+			return (uint64_t)-1;
+		}
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						conv,
+						NULL,
+						NULL,
+						smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_DISK_FREE(handle, conv_smb_fname, bsize, dfree, dsize);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_DISK_FREE(handle, smb_fname, bsize, dfree,
+					      dsize);
+	}
+}
+
+static int shadow_copy_zfs_get_quota(vfs_handle_struct *handle, const struct smb_filename *smb_fname,
+				  enum SMB_QUOTA_TYPE qtype, unid_t id,
+				  SMB_DISK_QUOTA *dq)
+{
+	int ret, saved_errno;
+	const char *gmt_start;
+	char *conv;
+	struct smb_filename *conv_smb_fname = NULL;
+
+	if (shadow_copy_zfs_match_name(smb_fname->base_name, &gmt_start)) {
+		conv = convert_shadow_zfs_name(handle, smb_fname->base_name, gmt_start, True);
+		if (conv == NULL) {
+			return -1;
+		}
+		conv_smb_fname = synthetic_smb_fname(talloc_tos(),
+						conv,
+						NULL,
+						NULL,
+						smb_fname->flags);
+		if (conv_smb_fname == NULL) {
+			TALLOC_FREE(conv);
+			return -1;
+		}
+
+		ret = SMB_VFS_NEXT_GET_QUOTA(handle, conv_smb_fname, qtype, id, dq);
+		saved_errno = errno;
+		TALLOC_FREE(conv);
+		TALLOC_FREE(conv_smb_fname);
+		errno = saved_errno;
+		return ret;
+	} else {
+		return SMB_VFS_NEXT_GET_QUOTA(handle, smb_fname, qtype, id, dq);
+	}
+}
+
+static int shadow_copy_zfs_connect(struct vfs_handle_struct *handle,
+				const char *service, const char *user)
+{
+	struct shadow_copy_zfs_config *config;
+	int ret;
+	const char *snapdir;
+	const char *gmt_format;
+	const char *sort_order;
+	const char *basedir = NULL;
+	const char *snapsharepath = NULL;
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
+	config = talloc_zero(handle->conn, struct shadow_copy_zfs_config);
+	if (config == NULL) {
+		DEBUG(0, ("talloc_zero() failed\n"));
+		errno = ENOMEM;
+		return -1;
+	}
+
+	config->dataset_path = lp_parm_const_string(SNUM(handle->conn),
+					  "shadow", "dataset", NULL);
+	if (config->dataset_path == NULL) {
+		DEBUG(0, ("dataset path not provided\n"));
+		errno = EINVAL;
+		return -1;
+	}
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
+	config->inclusions = lp_parm_string_list(SNUM(handle->conn), "shadow",
+						"include", empty_list);
+	config->exclusions = lp_parm_string_list(SNUM(handle->conn), "shadow",
+						 "exclude", empty_list);
+
+	DEBUG(10, ("shadow_copy_zfs_connect: configuration:\n"
+		   "  share root: '%s'\n"
+		   "  dataset path: '%s'\n"
+		   "  sort order: %s\n"
+		   "",
+		   handle->conn->connectpath,
+		   config->dataset_path,
+		   config->sort_order
+		   ));
+
+
+	SMB_VFS_HANDLE_SET_DATA(handle, config,
+				NULL, struct shadow_copy_zfs_config,
+				return -1);
+
+	return 0;
+}
+
+static struct vfs_fn_pointers vfs_shadow_copy_zfs_fns = {
+	.connect_fn = shadow_copy_zfs_connect,
+	.opendir_fn = shadow_copy_zfs_opendir,
+	.disk_free_fn = shadow_copy_zfs_disk_free,
+	.get_quota_fn = shadow_copy_zfs_get_quota,
+	.rename_fn = shadow_copy_zfs_rename,
+	.link_fn = shadow_copy_zfs_link,
+	.symlink_fn = shadow_copy_zfs_symlink,
+	.stat_fn = shadow_copy_zfs_stat,
+	.lstat_fn = shadow_copy_zfs_lstat,
+	.fstat_fn = shadow_copy_zfs_fstat,
+	.open_fn = shadow_copy_zfs_open,
+	.unlink_fn = shadow_copy_zfs_unlink,
+	.chmod_fn = shadow_copy_zfs_chmod,
+	.chown_fn = shadow_copy_zfs_chown,
+	.chdir_fn = shadow_copy_zfs_chdir,
+	.ntimes_fn = shadow_copy_zfs_ntimes,
+	.readlink_fn = shadow_copy_zfs_readlink,
+	.mknod_fn = shadow_copy_zfs_mknod,
+	.realpath_fn = shadow_copy_zfs_realpath,
+	.get_nt_acl_fn = shadow_copy_zfs_get_nt_acl,
+	.fget_nt_acl_fn = shadow_copy_zfs_fget_nt_acl,
+	.get_shadow_copy_data_fn = shadow_copy_zfs_get_shadow_copy_zfs_data,
+	.mkdir_fn = shadow_copy_zfs_mkdir,
+	.rmdir_fn = shadow_copy_zfs_rmdir,
+	.getxattr_fn = shadow_copy_zfs_getxattr,
+	.listxattr_fn = shadow_copy_zfs_listxattr,
+	.removexattr_fn = shadow_copy_zfs_removexattr,
+	.setxattr_fn = shadow_copy_zfs_setxattr,
+/*	.chmod_acl_fn = shadow_copy_zfs_chmod_acl, */
+	.chflags_fn = shadow_copy_zfs_chflags,
+	.get_real_filename_fn = shadow_copy_zfs_get_real_filename,
+	.connectpath_fn = shadow_copy_zfs_connectpath,
+};
+
+NTSTATUS vfs_shadow_copy_zfs_init(TALLOC_CTX *);
+NTSTATUS vfs_shadow_copy_zfs_init(TALLOC_CTX *ctx)
+{
+	return smb_register_vfs(SMB_VFS_INTERFACE_VERSION,
+				"shadow_copy_zfs", &vfs_shadow_copy_zfs_fns);
+}
