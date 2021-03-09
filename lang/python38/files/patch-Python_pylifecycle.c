--- Python/pylifecycle.c.orig	2020-12-21 16:25:24 UTC
+++ Python/pylifecycle.c
@@ -1196,6 +1196,16 @@ Py_FinalizeEx(void)
     runtime->initialized = 0;
     runtime->core_initialized = 0;
 
+    /* Destroy the state of all threads of the interpreter, except of the
+       current thread. In practice, only daemon threads should still be alive,
+       except if wait_for_thread_shutdown() has been cancelled by CTRL+C.
+       Clear frames of other threads to call objects destructors. Destructors
+       will be called in the current Python thread. Since
+       _PyRuntimeState_SetFinalizing() has been called, no other Python thread
+       can take the GIL at this point: if they try, they will exit
+       immediately. */
+    _PyThreadState_DeleteExcept(runtime, tstate);
+
     /* Flush sys.stdout and sys.stderr */
     if (flush_std_files() < 0) {
         status = -1;
