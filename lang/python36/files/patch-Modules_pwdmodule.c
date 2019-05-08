--- Modules/pwdmodule.c.orig	2019-05-08 16:52:34 UTC
+++ Modules/pwdmodule.c
@@ -116,8 +116,10 @@ static PyObject *
 pwd_getpwuid(PyObject *module, PyObject *uidobj)
 /*[clinic end generated code: output=c4ee1d4d429b86c4 input=ae64d507a1c6d3e8]*/
 {
+    PyObject *retval = NULL;
     uid_t uid;
     struct passwd *p;
+    char *buf;
 
     if (!_Py_Uid_Converter(uidobj, &uid)) {
         if (PyErr_ExceptionMatches(PyExc_OverflowError))
@@ -125,7 +127,35 @@ pwd_getpwuid(PyObject *module, PyObject 
                          "getpwuid(): uid not found");
         return NULL;
     }
-    if ((p = getpwuid(uid)) == NULL) {
+#ifdef HAVE_GETPWUID_R
+    Py_BEGIN_ALLOW_THREADS
+    int status, bufsize = NSS_BUFLEN_PASSWD;
+    struct passwd pwd;
+
+    buf = PyMem_RawMalloc(bufsize);
+
+    do {
+        status = getpwuid_r(uid, &pwd, buf, bufsize, &p);
+        if (p != NULL || status != ERANGE) {
+            break;
+        }
+        if ((bufsize << 1) > (NSS_BUFLEN_PASSWD << 10)) {
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
+    p = getpwuid(uid);
+#endif
+    if (p == NULL) {
         PyObject *uid_obj = _PyLong_FromUid(uid);
         if (uid_obj == NULL)
             return NULL;
@@ -134,7 +164,11 @@ pwd_getpwuid(PyObject *module, PyObject 
         Py_DECREF(uid_obj);
         return NULL;
     }
-    return mkpwent(p);
+    retval = mkpwent(p);
+#ifdef HAVE_GETPWUID_R
+    PyMem_RawFree(buf);
+#endif
+    return retval;
 }
 
 /*[clinic input]
@@ -152,7 +186,7 @@ static PyObject *
 pwd_getpwnam_impl(PyObject *module, PyObject *arg)
 /*[clinic end generated code: output=6abeee92430e43d2 input=d5f7e700919b02d3]*/
 {
-    char *name;
+    char *buf, *name;
     struct passwd *p;
     PyObject *bytes, *retval = NULL;
 
@@ -161,12 +195,43 @@ pwd_getpwnam_impl(PyObject *module, PyOb
     /* check for embedded null bytes */
     if (PyBytes_AsStringAndSize(bytes, &name, NULL) == -1)
         goto out;
-    if ((p = getpwnam(name)) == NULL) {
+#ifdef HAVE_GETPWNAM_R
+    Py_BEGIN_ALLOW_THREADS
+    int status, bufsize = NSS_BUFLEN_PASSWD;
+    struct passwd pwd;
+
+    buf = PyMem_RawMalloc(bufsize);
+
+    do {
+        status = getpwnam_r(name, &pwd, buf, bufsize, &p);
+        if (p != NULL || status != ERANGE) {
+            break;
+        }
+        if ((bufsize << 1) > (NSS_BUFLEN_PASSWD << 10)) {
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
+    p = getpwnam(name);
+#endif
+    if (p == NULL) {
         PyErr_Format(PyExc_KeyError,
                      "getpwnam(): name not found: %R", arg);
         goto out;
     }
     retval = mkpwent(p);
+#ifdef HAVE_GETPWNAM_R
+    PyMem_RawFree(buf);
+#endif
 out:
     Py_DECREF(bytes);
     return retval;
