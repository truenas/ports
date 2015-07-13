--- ./source3/modules/zfs_disk_free.h	1969-12-31 16:00:00.000000000 -0800
+++ ./source3/modules/zfs_disk_free.h	2015-07-13 14:41:59.000000000 -0700
@@ -0,0 +1,33 @@
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
+#ifndef	__ZFS_DISK_FREE_H
+#define	__ZFS_DISK_FREE_H
+
+uint64_t smb_zfs_disk_free(char *path, uint64_t *bsize, uint64_t *dfree, uint64_t *dsize);
+
+#endif	/* !__ZFS_DISK_FREE_H */
