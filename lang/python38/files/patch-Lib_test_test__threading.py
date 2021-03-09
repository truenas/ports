--- Lib/test/test_threading.py.orig	2020-12-21 16:25:24 UTC
+++ Lib/test/test_threading.py
@@ -762,6 +762,44 @@ class ThreadTests(BaseTestCase):
                 # Daemon threads must never add it to _shutdown_locks.
                 self.assertNotIn(tstate_lock, threading._shutdown_locks)
 
+    def test_locals_at_exit(self):
+        # bpo-19466: thread locals must not be deleted before destructors
+        # are called
+        rc, out, err = assert_python_ok("-c", """if 1:
+            import threading
+            class Atexit:
+                def __del__(self):
+                    print("thread_dict.atexit = %r" % thread_dict.atexit)
+            thread_dict = threading.local()
+            thread_dict.atexit = "value"
+            atexit = Atexit()
+        """)
+        self.assertEqual(out.rstrip(), b"thread_dict.atexit = 'value'")
+
+    def test_warnings_at_exit(self):
+        # bpo-19466: try to call most destructors at Python shutdown before
+        # destroying Python thread states
+        filename = __file__
+        rc, out, err = assert_python_ok("-Wd", "-c", """if 1:
+            import time
+            import threading
+            from test import support
+            def open_sleep():
+                # a warning will be emitted when the open file will be
+                # destroyed (without being explicitly closed) while the daemon
+                # thread is destroyed
+                fileobj = open(%a, 'rb')
+                start_event.set()
+                time.sleep(support.LONG_TIMEOUT)
+            start_event = threading.Event()
+            thread = threading.Thread(target=open_sleep, daemon=True)
+            thread.start()
+            # wait until the thread started
+            start_event.wait()
+        """ % filename)
+        self.assertRegex(err.rstrip(),
+                         b"^sys:1: ResourceWarning: unclosed file ")
+
 
 class ThreadJoinOnShutdown(BaseTestCase):
 
