--- lib/tzlocal/unix.py.orig	2018-05-22 14:43:58.358640000 -0400
+++ lib/tzlocal/unix.py	2018-05-22 14:31:02.824064000 -0400
@@ -45,7 +45,7 @@
     # Now look for distribution specific configuration files
     # that contain the timezone name.
     tzpath = os.path.join(_root, 'etc/timezone')
-    if os.path.exists(tzpath):
+    if os.path.exists(tzpath) and os.path.getsize(tzpath) > 0:
         with open(tzpath, 'rb') as tzfile:
             data = tzfile.read()
 
