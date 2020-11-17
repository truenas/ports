--- psutil/__init__.py.orig
+++ psutil/__init__.py
@@ -799,6 +799,16 @@ class Process(object):
             """
             return self._proc.rlimit(resource, limits)
 
+    # FreeBSD only
+    if hasattr(_psplatform.Process, "jid"):
+
+        def jid(self):
+            """Returns process jail ID.
+
+            Available on FreeBSD only.
+            """
+            return self._proc.jid()
+
     # Windows, Linux and FreeBSD only
     if hasattr(_psplatform.Process, "cpu_affinity_get"):
 
