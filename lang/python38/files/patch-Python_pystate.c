--- Python/pystate.c.orig	2020-12-21 11:25:24.000000000 -0500
+++ Python/pystate.c	2021-03-07 23:09:59.733061000 -0500
@@ -802,6 +802,10 @@
     Py_CLEAR(tstate->async_gen_finalizer);
 
     Py_CLEAR(tstate->context);
+
+    if (tstate->on_delete != NULL) {
+	tstate->on_delete(tstate->on_delete_data);
+    }
 }
 
 
@@ -824,9 +828,7 @@
     if (tstate->next)
         tstate->next->prev = tstate->prev;
     HEAD_UNLOCK(runtime);
-    if (tstate->on_delete != NULL) {
-        tstate->on_delete(tstate->on_delete_data);
-    }
+
     PyMem_RawFree(tstate);
 }
 
@@ -890,25 +892,30 @@
 _PyThreadState_DeleteExcept(_PyRuntimeState *runtime, PyThreadState *tstate)
 {
     PyInterpreterState *interp = tstate->interp;
-    PyThreadState *p, *next, *garbage;
+
     HEAD_LOCK(runtime);
     /* Remove all thread states, except tstate, from the linked list of
        thread states.  This will allow calling PyThreadState_Clear()
        without holding the lock. */
-    garbage = interp->tstate_head;
-    if (garbage == tstate)
-        garbage = tstate->next;
-    if (tstate->prev)
+    PyThreadState *list = interp->tstate_head;
+    if (list == tstate) {
+        list = tstate->next;
+    }
+    if (tstate->prev) {
         tstate->prev->next = tstate->next;
-    if (tstate->next)
+    }
+    if (tstate->next) {
         tstate->next->prev = tstate->prev;
+    }
     tstate->prev = tstate->next = NULL;
     interp->tstate_head = tstate;
     HEAD_UNLOCK(runtime);
+
     /* Clear and deallocate all stale thread states.  Even if this
        executes Python code, we should be safe since it executes
        in the current thread, not one of the stale threads. */
-    for (p = garbage; p; p = next) {
+    PyThreadState *p, *next;
+    for (p = list; p; p = next) {
         next = p->next;
         PyThreadState_Clear(p);
         PyMem_RawFree(p);
