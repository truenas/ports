--- ./source3/modules/vfs_zfs_space.c	1969-12-31 16:00:00.000000000 -0800
+++ ./source3/modules/vfs_zfs_space.c	2015-07-13 14:41:48.000000000 -0700
@@ -0,0 +1,66 @@
+/*-
+ * Copyright 2015 iXsystems, Inc.
+ * All rights reserved
+ *
+ * Redistribution and use in source and binary forms, with or without
+ * modification, are permitted providing that the following conditions
+ * are met:
+ * 1. Redistributions of source code must retain the above copyright
+ *    notice, this list of conditions and the following disclaimer.
+ * 2. Redistributions in binary form must reproduce the above copyright
+ *    notice, this list of conditions and the following disclaimer in the
+ *    documentation and/or other materials provided with the distribution.
+ *
+ * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
+ * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
+ * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
+ * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
+ * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
+ * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
+ * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
+ * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
+ * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
+ * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
+ * POSSIBILITY OF SUCH DAMAGE.
+ *
+ */
+
+#include "includes.h"
+#include "system/filesys.h"
+#include "lib/util/tevent_ntstatus.h"
+
+#include "modules/zfs_disk_free.h"
+
+
+static uint64_t vfs_zfs_space_disk_free(vfs_handle_struct *handle, const char *path,
+	bool small_query, uint64_t *bsize, uint64_t *dfree, uint64_t *dsize)
+{
+	uint64_t res;
+	char rp[PATH_MAX] = { 0 };
+
+	if (realpath(path, rp) == NULL)
+		return (-1);
+
+	DEBUG(9, ("realpath = %s\n", rp));
+
+	res = smb_zfs_disk_free(rp, bsize, dfree, dsize, debugfunc);
+	if (res == (uint64_t)-1)
+		return (res);
+
+	DEBUG(9, ("*bsize = %" PRIu64 "\n", *bsize));
+	DEBUG(9, ("*dfree = %" PRIu64 "\n", *dfree));
+	DEBUG(9, ("*dsize = %" PRIu64 "\n", *dsize));
+
+	return (res);
+}
+
+static struct vfs_fn_pointers vfs_zfs_space_fns = {
+	.disk_free_fn = vfs_zfs_space_disk_free
+};
+
+NTSTATUS vfs_zfs_space_init(void);
+NTSTATUS vfs_zfs_space_init(void)
+{
+	return smb_register_vfs(SMB_VFS_INTERFACE_VERSION,
+		"zfs_space", &vfs_zfs_space_fns);
+}
