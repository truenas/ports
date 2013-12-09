--- ./Modules/_ctypes/libffi/src/dlmalloc.c.orig	2013-12-09 13:14:37.594635872 -0800
+++ ./Modules/_ctypes/libffi/src/dlmalloc.c	2013-12-09 13:14:49.449635911 -0800
@@ -2528,8 +2528,8 @@
 #if USE_DEV_RANDOM
       int fd;
       unsigned char buf[sizeof(size_t)];
-      /* Try to use /dev/urandom, else fall back on using time */
-      if ((fd = open("/dev/urandom", O_RDONLY)) >= 0 &&
+      /* Try to use /dev/random, else fall back on using time */
+      if ((fd = open("/dev/random", O_RDONLY)) >= 0 &&
           read(fd, buf, sizeof(buf)) == sizeof(buf)) {
         s = *((size_t *) buf);
         close(fd);
