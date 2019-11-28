--- ws4py/websocket.py.orig	2019-11-29 17:25:00 UTC
+++ ws4py/websocket.py
@@ -222,11 +222,21 @@ class WebSocket(object):
         """
         if self.sock:
             try:
+                """
+                On TrueNAS HA systems, if the other controller is
+                offline, has hardware failure, turned off etc etc
+                then the sock.shutdown() call can throw an exception of
+                "ERRNO 57 Socket is not connected" which makes sense
+                in this scenario. Because this happens, sock.close()
+                never runs which leaves a fd opened up for the socket.
+                Ensure that sock.close() gets run by moving it to the
+                finally clause.
+                """
                 self.sock.shutdown(socket.SHUT_RDWR)
-                self.sock.close()
             except:
                 pass
             finally:
+                self.sock.close()
                 self.sock = None

     def ping(self, message):
