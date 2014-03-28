--- python/types.c.orig	2009-07-30 08:24:34.000000000 -0700
+++ python/types.c	2014-03-28 14:23:39.000000000 -0700
@@ -149,6 +149,14 @@
     return (ret);
 }
 
+static void
+libxml_xmlDocPtrWrap_dtor(void *cobj, void *descr)
+{
+	xmlDocPtr doc = cobj;
+
+	xmlFreeDoc(doc);
+}
+
 PyObject *
 libxml_xmlDocPtrWrap(xmlDocPtr doc)
 {
@@ -161,10 +169,9 @@
         Py_INCREF(Py_None);
         return (Py_None);
     }
-    /* TODO: look at deallocation */
     ret =
         PyCObject_FromVoidPtrAndDesc((void *) doc, (char *) "xmlDocPtr",
-                                     NULL);
+                                     &libxml_xmlDocPtrWrap_dtor);
     return (ret);
 }
 
