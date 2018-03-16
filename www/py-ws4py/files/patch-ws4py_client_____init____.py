--- ws4py/client/__init__.py.orig	2018-03-16 14:28:29 UTC
+++ ws4py/client/__init__.py
@@ -103,7 +103,6 @@ class WebSocketBaseClient(WebSocket):
 
             sock = socket.socket(family, socktype, proto)
             sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
-            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
             if hasattr(socket, 'AF_INET6') and family == socket.AF_INET6 and \
               self.host.startswith('::'):
                 try:
