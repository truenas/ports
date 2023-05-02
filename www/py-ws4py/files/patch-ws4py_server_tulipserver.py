--- ws4py/server/tulipserver.py.orig	2023-04-28 13:58:42 UTC
+++ ws4py/server/tulipserver.py
@@ -40,7 +40,7 @@ class WebSocketProtocol(asyncio.StreamReaderProtocol):
         #self.stream.set_transport(transport)
         asyncio.StreamReaderProtocol.connection_made(self, transport)
         # Let make it concurrent for others to tag along
-        f = asyncio.async(self.handle_initial_handshake())
+        f = asyncio.ensure_future(self.handle_initial_handshake())
         f.add_done_callback(self.terminated)
 
     @property
