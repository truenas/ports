--- ws4py/async_websocket.py.orig	2023-04-28 13:54:11 UTC
+++ ws4py/async_websocket.py
@@ -84,7 +84,7 @@ class WebSocket(_WebSocket):
         def closeit():
             yield from self.proto.writer.drain()
             self.proto.writer.close()
-        asyncio.async(closeit())
+        asyncio.ensure_future(closeit())
 
     def _write(self, data):
         """
@@ -94,7 +94,7 @@ class WebSocket(_WebSocket):
         def sendit(data):
             self.proto.writer.write(data)
             yield from self.proto.writer.drain()
-        asyncio.async(sendit(data))
+        asyncio.ensure_future(sendit(data))
 
     @asyncio.coroutine
     def run(self):
