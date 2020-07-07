--- libatalk/util/pidfile.c.orig	2020-05-15 08:36:18.429541604 -0700
+++ libatalk/util/pidfile.c	2020-05-15 08:45:34.205558383 -0700
@@ -0,0 +1,267 @@
+/*
+   Unix SMB/CIFS implementation.
+   pidfile handling
+   Copyright (C) Andrew Tridgell 1998
+   Copyright (C) Amitay Isaccs  2016
+
+   This program is free software; you can redistribute it and/or modify
+   it under the terms of the GNU General Public License as published by
+   the Free Software Foundation; either version 3 of the License, or
+   (at your option) any later version.
+
+   This program is distributed in the hope that it will be useful,
+   but WITHOUT ANY WARRANTY; without even the implied warranty of
+   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
+   GNU General Public License for more details.
+
+   You should have received a copy of the GNU General Public License
+   along with this program.  If not, see <http://www.gnu.org/licenses/>.
+*/
+
+#ifdef HAVE_CONFIG_H
+#include "config.h"
+#endif /* HAVE_CONFIG_H */
+
+#include <errno.h>
+#include <fcntl.h>
+#include <string.h>
+#include <strings.h>
+#include <stdio.h>
+#include <stdbool.h>
+#include <stdlib.h>
+#include <unistd.h>
+
+#include <atalk/util.h>
+
+#ifndef ZERO_STRUCT
+#define ZERO_STRUCT(x) memset_s((char *)&(x), sizeof(x), 0, sizeof(x))
+#endif
+
+static bool fcntl_lock(int fd, int op, off_t offset, off_t count, int type)
+{
+        struct flock lock;
+        int ret;
+
+        lock.l_type = type;
+        lock.l_whence = SEEK_SET;
+        lock.l_start = offset;
+        lock.l_len = count;
+        lock.l_pid = 0;
+
+        ret = fcntl(fd,op,&lock);
+
+        if (op == F_GETLK) {
+                if ((ret != -1) &&
+                                (lock.l_type != F_UNLCK) &&
+                                (lock.l_pid != 0) &&
+                                (lock.l_pid != getpid())) {
+                        return true;
+                }
+
+                /* it must be not locked or locked by me */
+                return false;
+        }
+
+        /* a lock set or unset */
+        if (ret == -1) {
+                return false;
+        }
+        return true;
+}
+
+
+static bool set_close_on_exec(int fd)
+{
+#ifdef FD_CLOEXEC
+        int val;
+
+        val = fcntl(fd, F_GETFD, 0);
+        if (val >= 0) {
+                val |= FD_CLOEXEC;
+                val = fcntl(fd, F_SETFD, val);
+                if (val != -1) {
+                        return true;
+                }
+        }
+#endif
+        return false;
+}
+
+
+int pidfile_path_create(const char *path, int *outfd)
+{
+	struct flock lck;
+	char tmp[64] = { 0 };
+	pid_t pid;
+	int fd, ret = 0;
+	int len;
+	ssize_t nwritten;
+
+	pid = getpid();
+
+	fd = open(path, O_CREAT|O_WRONLY|O_NONBLOCK, 0644);
+	if (fd == -1) {
+		return errno;
+	}
+
+	if (! set_close_on_exec(fd)) {
+		close(fd);
+		return EIO;
+	}
+
+	lck = (struct flock) {
+		.l_type = F_WRLCK,
+		.l_whence = SEEK_SET,
+	};
+
+	do {
+		ret = fcntl(fd, F_SETLK, &lck);
+	} while ((ret == -1) && (errno == EINTR));
+
+	if (ret != 0) {
+		ret = errno;
+		close(fd);
+		return ret;
+	}
+
+	/*
+	 * PID file is locked by us so from here on we should unlink
+	 * on failure
+	 */
+
+	do {
+		ret = ftruncate(fd, 0);
+	} while ((ret == -1) && (errno == EINTR));
+
+	if (ret == -1) {
+		ret = EIO;
+		goto fail_unlink;
+	}
+
+	len = snprintf(tmp, sizeof(tmp), "%u\n", pid);
+	if (len < 0) {
+		ret = errno;
+		goto fail_unlink;
+	}
+	if ((size_t)len >= sizeof(tmp)) {
+		ret = ENOSPC;
+		goto fail_unlink;
+	}
+
+	do {
+		nwritten = write(fd, tmp, len);
+	} while ((nwritten == -1) && (errno == EINTR));
+
+	if ((nwritten == -1) || (nwritten != len)) {
+		ret = EIO;
+		goto fail_unlink;
+	}
+
+	if (outfd != NULL) {
+		*outfd = fd;
+	}
+	return 0;
+
+fail_unlink:
+	unlink(path);
+	close(fd);
+	return ret;
+}
+
+void pidfile_fd_close(int fd)
+{
+	struct flock lck = {
+		.l_type = F_UNLCK,
+		.l_whence = SEEK_SET,
+	};
+	int ret;
+
+	do {
+		ret = fcntl(fd, F_SETLK, &lck);
+	} while ((ret == -1) && (errno == EINTR));
+
+	do {
+		ret = close(fd);
+	} while ((ret == -1) && (errno == EINTR));
+}
+
+
+/**
+ * return the pid in a pidfile. return 0 if the process (or pidfile)
+ * does not exist
+ */
+pid_t pidfile_pid(const char *piddir, const char *name)
+{
+	size_t len = strlen(piddir) + strlen(name) + 6;
+	char pidFile[len];
+	int fd;
+	char pidstr[20];
+	pid_t ret = -1;
+
+	snprintf(pidFile, sizeof(pidFile), "%s/%s.pid", piddir, name);
+
+	fd = open(pidFile, O_NONBLOCK | O_RDONLY, 0644);
+
+	if (fd == -1) {
+		return 0;
+	}
+
+	ZERO_STRUCT(pidstr);
+
+	if (read(fd, pidstr, sizeof(pidstr)-1) <= 0) {
+		goto noproc;
+	}
+
+	ret = (pid_t)atoi(pidstr);
+	if (ret <= 0) {
+		goto noproc;
+	}
+
+	if (fcntl_lock(fd,F_SETLK,0,1,F_RDLCK)) {
+		/* we could get the lock - it can't be a Samba process */
+		goto noproc;
+	}
+
+	close(fd);
+	return ret;
+
+ noproc:
+	close(fd);
+	return 0;
+}
+
+/**
+ * create a pid file in the pid directory. open it and leave it locked
+ */
+void pidfile_create(const char *piddir, const char *name)
+{
+	size_t len = strlen(piddir) + strlen(name) + 6;
+	char pidFile[len];
+	pid_t pid;
+	int ret;
+
+	snprintf(pidFile, sizeof(pidFile), "%s/%s.pid", piddir, name);
+
+	pid = pidfile_pid(piddir, name);
+	if (pid != 0) {
+		exit(1);
+	}
+
+	ret = pidfile_path_create(pidFile, NULL);
+	if (ret != 0) {
+		exit(1);
+	}
+
+	/* Leave pid file open & locked for the duration... */
+}
+
+void pidfile_unlink(const char *piddir, const char *name)
+{
+	int ret;
+	char *pidFile = NULL;
+
+	if (asprintf(&pidFile, "%s/%s.pid", piddir, name) < 0) {
+		exit(1);
+	}
+	ret = unlink(pidFile);
+}
