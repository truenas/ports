--- geventwebsocket/handler.py.orig	2015-05-26 07:11:50 UTC
+++ geventwebsocket/handler.py
@@ -1,5 +1,6 @@
 import base64
 import hashlib
+import six
 import warnings
 
 from gevent.pywsgi import WSGIHandler
@@ -212,11 +213,13 @@ class WebSocketHandler(WSGIHandler):
             'wsgi.websocket': self.websocket
         })
 
+        sec_accept = base64.b64encode(hashlib.sha1(six.b(key + self.GUID)).digest())
+        if six.PY3:
+            sec_accept = sec_accept.decode('utf-8')
         headers = [
             ("Upgrade", "websocket"),
             ("Connection", "Upgrade"),
-            ("Sec-WebSocket-Accept", base64.b64encode(
-                hashlib.sha1(key + self.GUID).digest())),
+            ("Sec-WebSocket-Accept", sec_accept),
         ]
 
         if protocol:
@@ -233,7 +236,7 @@ class WebSocketHandler(WSGIHandler):
         return self.server.logger
 
     def log_request(self):
-        if '101' not in self.status:
+        if '101' not in self.status.decode('utf-8'):
             self.logger.info(self.format_request())
 
     @property
