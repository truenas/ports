--- psutil/_psbsd.py.orig
+++ psutil/_psbsd.py
@@ -901,3 +901,7 @@ class Process(object):
         @wrap_exceptions
         def memory_maps(self):
             return cext.proc_memory_maps(self.pid)
+
+        @wrap_exceptions
+        def jid(self):
+            return cext.proc_jail_id(self.pid)
