diff --git a/ws4py/websocket.py b/ws4py/websocket.py
index 61f8c33..3614f80 100644
--- a/ws4py/websocket.py
+++ b/ws4py/websocket.py
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
