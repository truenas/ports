--- netifaces.c.orig	2020-04-16 12:23:38 UTC
+++ netifaces.c
@@ -2079,18 +2079,24 @@ gateways (PyObject *self)

         if (string_from_sockaddr (sa, strbuf, sizeof(strbuf)) == 0) {
           PyObject *pyaddr = PyUnicode_FromString (strbuf);
+          if (pyaddr == NULL) {
+            PyErr_Clear();
+          }
 #ifdef RTF_IFSCOPE
           PyObject *isdefault = PyBool_FromLong (!(msg->rtm_flags & RTF_IFSCOPE));
 #else
           Py_INCREF(Py_True);
           PyObject *isdefault = Py_True;
 #endif
-          tuple = PyTuple_Pack (3, pyaddr, pyifname, isdefault);
+          if (pyaddr != NULL) {
+            tuple = PyTuple_Pack (3, pyaddr, pyifname, isdefault);
+            if (PyObject_IsTrue (isdefault)) {
+              deftuple = PyTuple_Pack (2, pyaddr, pyifname);
+            }

-          if (PyObject_IsTrue (isdefault))
-            deftuple = PyTuple_Pack (2, pyaddr, pyifname);
+            Py_DECREF (pyaddr);
+          }

-          Py_DECREF (pyaddr);
           Py_DECREF (isdefault);
         }

