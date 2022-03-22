--- paramiko/util.py.orig
+++ paramiko/util.py
@@ -225,24 +225,20 @@ def mod_inverse(x, m):
     return u2
 
 
-_g_thread_ids = {}
+_g_thread_data = threading.local()
 _g_thread_counter = 0
 _g_thread_lock = threading.Lock()
 
 
 def get_thread_id():
-    global _g_thread_ids, _g_thread_counter, _g_thread_lock
-    tid = id(threading.currentThread())
+    global _g_thread_data, _g_thread_counter, _g_thread_lock
     try:
-        return _g_thread_ids[tid]
-    except KeyError:
-        _g_thread_lock.acquire()
-        try:
+        return _g_thread_data.id
+    except AttributeError:
+        with _g_thread_lock:
             _g_thread_counter += 1
-            ret = _g_thread_ids[tid] = _g_thread_counter
-        finally:
-            _g_thread_lock.release()
-        return ret
+            _g_thread_data.id = _g_thread_counter
+        return _g_thread_data.id
 
 
 def log_to_file(filename, level=DEBUG):
