--- Python/random.c.orig	2013-12-09 13:09:22.155659223 -0800
+++ Python/random.c	2013-12-09 13:11:51.934646938 -0800
@@ -118,7 +118,7 @@
 
 #if !defined(MS_WINDOWS) && !defined(__VMS)
 
-/* Read size bytes from /dev/urandom into buffer.
+/* Read size bytes from /dev/random into buffer.
    Call Py_FatalError() on error. */
 static void
 dev_urandom_noraise(char *buffer, Py_ssize_t size)
@@ -128,9 +128,9 @@
 
     assert (0 < size);
 
-    fd = open("/dev/urandom", O_RDONLY);
+    fd = open("/dev/random", O_RDONLY);
     if (fd < 0)
-        Py_FatalError("Failed to open /dev/urandom");
+        Py_FatalError("Failed to open /dev/random");
 
     while (0 < size)
     {
@@ -140,7 +140,7 @@
         if (n <= 0)
         {
             /* stop on error or if read(size) returned 0 */
-            Py_FatalError("Failed to read bytes from /dev/urandom");
+            Py_FatalError("Failed to read bytes from /dev/random");
             break;
         }
         buffer += n;
@@ -161,12 +161,12 @@
         return 0;
 
     Py_BEGIN_ALLOW_THREADS
-    fd = open("/dev/urandom", O_RDONLY);
+    fd = open("/dev/random", O_RDONLY);
     Py_END_ALLOW_THREADS
     if (fd < 0)
     {
         PyErr_SetString(PyExc_NotImplementedError,
-                        "/dev/urandom (or equivalent) not found");
+                        "/dev/random (or equivalent) not found");
         return -1;
     }
 
@@ -189,7 +189,7 @@
             PyErr_SetFromErrno(PyExc_OSError);
         else
             PyErr_Format(PyExc_RuntimeError,
-                         "Failed to read %zi bytes from /dev/urandom",
+                         "Failed to read %zi bytes from /dev/random",
                          size);
         close(fd);
         return -1;
