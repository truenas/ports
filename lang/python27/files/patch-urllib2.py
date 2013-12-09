--- ./Lib/urllib2.py.orig	2013-12-09 13:13:35.645640020 -0800
+++ ./Lib/urllib2.py	2013-12-09 13:13:52.067701376 -0800
@@ -917,11 +917,11 @@
 
 def randombytes(n):
     """Return n random bytes."""
-    # Use /dev/urandom if it is available.  Fall back to random module
+    # Use /dev/random if it is available.  Fall back to random module
     # if not.  It might be worthwhile to extend this function to use
     # other platform-specific mechanisms for getting random bytes.
-    if os.path.exists("/dev/urandom"):
-        f = open("/dev/urandom")
+    if os.path.exists("/dev/random"):
+        f = open("/dev/random")
         s = f.read(n)
         f.close()
         return s
