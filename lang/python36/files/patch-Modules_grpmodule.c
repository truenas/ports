--- Modules/grpmodule.c.orig	2019-05-08 16:52:12 UTC
+++ Modules/grpmodule.c
@@ -96,7 +96,8 @@ static PyObject *
 grp_getgrgid_impl(PyObject *module, PyObject *id)
 /*[clinic end generated code: output=30797c289504a1ba input=15fa0e2ccf5cda25]*/
 {
-    PyObject *py_int_id;
+    PyObject *py_int_id, *retval = NULL;
+    char *buf;
     gid_t gid;
     struct group *p;
 
@@ -119,8 +120,35 @@ grp_getgrgid_impl(PyObject *module, PyOb
         }
         Py_DECREF(py_int_id);
     }
+#ifdef HAVE_GETGRGID_R
+    Py_BEGIN_ALLOW_THREADS
+    int status, bufsize = NSS_BUFLEN_GROUP;
+    struct group grp;
 
-    if ((p = getgrgid(gid)) == NULL) {
+    buf = PyMem_RawMalloc(bufsize);
+
+    do {
+        status = getgrgid_r(gid, &grp, buf, bufsize, &p);
+        if (p != NULL || status != ERANGE) {
+            break;
+        }
+        if ((bufsize << 1) > (NSS_BUFLEN_GROUP << 10)) {
+            status = ERANGE;
+            break;
+        }
+        bufsize <<= 1;
+        buf = PyMem_RawRealloc(buf, bufsize);
+    } while (1);
+
+    if (status != 0 || p == NULL) {
+        PyMem_RawFree(buf);
+        p = NULL;
+    }
+    Py_END_ALLOW_THREADS
+#else
+    p = getgrgid(gid);
+#endif
+    if (p == NULL) {
         PyObject *gid_obj = _PyLong_FromGid(gid);
         if (gid_obj == NULL)
             return NULL;
@@ -128,7 +156,11 @@ grp_getgrgid_impl(PyObject *module, PyOb
         Py_DECREF(gid_obj);
         return NULL;
     }
-    return mkgrent(p);
+    retval = mkgrent(p);
+#ifdef HAVE_GETGRGID_R
+    PyMem_RawFree(buf);
+#endif
+    return retval;
 }
 
 /*[clinic input]
@@ -145,7 +177,7 @@ static PyObject *
 grp_getgrnam_impl(PyObject *module, PyObject *name)
 /*[clinic end generated code: output=67905086f403c21c input=08ded29affa3c863]*/
 {
-    char *name_chars;
+    char *buf, *name_chars;
     struct group *p;
     PyObject *bytes, *retval = NULL;
 
@@ -154,12 +186,42 @@ grp_getgrnam_impl(PyObject *module, PyOb
     /* check for embedded null bytes */
     if (PyBytes_AsStringAndSize(bytes, &name_chars, NULL) == -1)
         goto out;
+#ifdef HAVE_GETGRNAM_R
+    Py_BEGIN_ALLOW_THREADS
+    int status, bufsize = NSS_BUFLEN_GROUP;
+    struct group grp;
 
-    if ((p = getgrnam(name_chars)) == NULL) {
+    buf = PyMem_RawMalloc(bufsize);
+
+    do {
+        status = getgrnam_r(name_chars, &grp, buf, bufsize, &p);
+        if (p != NULL || status != ERANGE) {
+            break;
+        }
+        if ((bufsize << 1) > (NSS_BUFLEN_GROUP << 10)) {
+            status = ERANGE;
+            break;
+        }
+        bufsize <<= 1;
+        buf = PyMem_RawRealloc(buf, bufsize);
+    } while (1);
+
+    if (status != 0 || p == NULL) {
+        PyMem_RawFree(buf);
+        p = NULL;
+    }
+    Py_END_ALLOW_THREADS
+#else
+    p = getgrnam(name_chars);
+#endif
+    if (p == NULL) {
         PyErr_Format(PyExc_KeyError, "getgrnam(): name not found: %R", name);
         goto out;
     }
     retval = mkgrent(p);
+#ifdef HAVE_GETGRNAM_R
+    PyMem_RawFree(buf);
+#endif
 out:
     Py_DECREF(bytes);
     return retval;
