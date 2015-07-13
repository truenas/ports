--- ./source3/modules/zfs_disk_free.c	1969-12-31 16:00:00.000000000 -0800
+++ ./source3/modules/zfs_disk_free.c	2015-07-13 15:37:57.000000000 -0700
@@ -0,0 +1,76 @@
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
+#define NEED_SOLARIS_BOOLEAN
+
+#include <libzfs.h>
+
+#include "modules/zfs_disk_free.h"
+
+
+uint64_t
+smb_zfs_disk_free(char *path, uint64_t *bsize, uint64_t *dfree, uint64_t *dsize)
+{
+	size_t blocksize = 1024;
+	libzfs_handle_t *libzfsp;
+	zfs_handle_t *zfsp;
+	uint64_t available, usedbysnapshots, usedbydataset,
+		usedbychildren, usedbyrefreservation, real_used, total;
+
+	if (path == NULL)
+		return (-1);
+
+	if ((libzfsp = libzfs_init()) == NULL)
+		return (-1);
+
+	libzfs_print_on_error(libzfsp, B_TRUE);
+
+	zfsp = zfs_path_to_zhandle(libzfsp, path,
+		ZFS_TYPE_VOLUME|ZFS_TYPE_DATASET|ZFS_TYPE_FILESYSTEM);
+	if (zfsp == NULL)
+		return (-1);
+
+	available = zfs_prop_get_int(zfsp, ZFS_PROP_AVAILABLE);
+	usedbysnapshots = zfs_prop_get_int(zfsp, ZFS_PROP_USEDSNAP);
+	usedbydataset = zfs_prop_get_int(zfsp, ZFS_PROP_USEDDS);
+	usedbychildren = zfs_prop_get_int(zfsp, ZFS_PROP_USEDCHILD);
+	usedbyrefreservation = zfs_prop_get_int(zfsp, ZFS_PROP_USEDREFRESERV);
+
+	zfs_close(zfsp);
+	libzfs_fini(libzfsp);
+
+	real_used = usedbysnapshots + usedbydataset + usedbychildren;
+
+	total = (real_used + available) / blocksize;
+	available /= blocksize;
+
+	*bsize = blocksize;
+	*dfree = available;
+	*dsize = total;
+
+	return (*dfree);	
+}
