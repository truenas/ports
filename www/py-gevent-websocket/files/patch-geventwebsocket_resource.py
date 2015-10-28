--- geventwebsocket/resource.py.orig	2015-05-27 07:21:53 UTC
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
@@ -63,7 +64,7 @@ class Resource(object):
 
     def _app_by_path(self, environ_path, is_websocket_request):
         # Which app matched the current path?
-        for path, app in self.apps.items():
+        for path, app in self.apps:
             if re.match(path, environ_path):
                 if is_websocket_request == self._is_websocket_app(app):
                     return app
