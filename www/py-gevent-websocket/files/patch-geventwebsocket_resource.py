--- geventwebsocket/resource.py.orig	2015-10-28 10:44:39 UTC
+++ geventwebsocket/resource.py
@@ -1,4 +1,5 @@
 import re
+import six
 import warnings
 
 from collections import OrderedDict
@@ -51,7 +52,7 @@ class Resource(object):
             # Convert to a list of tuples
             # The order is undefined, which can be very bad, but this keeps
             # backwards compatibility.
-            self.apps = [(path, app) for path, app in apps.iteritems()]
+            self.apps = [(path, app) for path, app in six.iteritems(apps)]
 
     # An app can either be a standard WSGI application (an object we call with
     # __call__(self, environ, start_response)) or a class we instantiate
