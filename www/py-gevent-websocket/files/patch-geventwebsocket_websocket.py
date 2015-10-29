--- geventwebsocket/websocket.py.orig	2014-01-11 22:04:37 UTC
+++ geventwebsocket/websocket.py
@@ -1,3 +1,4 @@
+import six
 import struct
 
 from socket import error
@@ -76,11 +77,11 @@ class WebSocket(object):
         :returns: The utf-8 byte string equivalent of `text`.
         """
 
-        if isinstance(text, str):
+        if isinstance(text, six.binary_type):
             return text
 
-        if not isinstance(text, unicode):
-            text = unicode(text or '')
+        if not isinstance(text, six.text_type):
+            text = six.text_type(text or '')
 
         return text.encode('utf-8')
 
@@ -166,7 +167,7 @@ class WebSocket(object):
             raise ProtocolError('Invalid close frame: {0} {1}'.format(
                 header, payload))
 
-        code = struct.unpack('!H', str(payload[:2]))[0]
+        code = struct.unpack(b'!H', payload[:2])[0]
         payload = payload[2:]
 
         if payload:
@@ -203,15 +204,15 @@ class WebSocket(object):
             raise ProtocolError
 
         if not header.length:
-            return header, ''
+            return header, b''
 
         try:
             payload = self.raw_read(header.length)
         except error:
-            payload = ''
+            payload = b''
         except Exception:
             # TODO log out this exception
-            payload = ''
+            payload = b''
 
         if len(payload) != header.length:
             raise WebSocketError('Unexpected EOF reading frame payload')
@@ -238,7 +239,7 @@ class WebSocket(object):
         if an exception is called. Use `receive` instead.
         """
         opcode = None
-        message = ""
+        message = b""
 
         while True:
             header, payload = self.read_frame()
@@ -323,7 +324,7 @@ class WebSocket(object):
         if opcode == self.OPCODE_TEXT:
             message = self._encode_bytes(message)
         elif opcode == self.OPCODE_BINARY:
-            message = str(message)
+            message = six.binary_type(message)
 
         header = Header.encode_header(True, opcode, '', len(message), 0)
 
@@ -337,7 +338,7 @@ class WebSocket(object):
         Send a frame over the websocket with message as its payload
         """
         if binary is None:
-            binary = not isinstance(message, (str, unicode))
+            binary = not isinstance(message, six.string_types)
 
         opcode = self.OPCODE_BINARY if binary else self.OPCODE_TEXT
 
@@ -353,7 +354,6 @@ class WebSocket(object):
         message.  The underlying socket object is _not_ closed, that is the
         responsibility of the initiator.
         """
-
         if self.closed:
             self.current_app.on_close(MSG_ALREADY_CLOSED)
 
@@ -361,7 +361,7 @@ class WebSocket(object):
             message = self._encode_bytes(message)
 
             self.send_frame(
-                struct.pack('!H%ds' % len(message), code, message),
+                struct.pack(six.b('!H%ds' % len(message)), code, message),
                 opcode=self.OPCODE_CLOSE)
         except WebSocketError:
             # Failed to write the closing frame but it's ok because we're
@@ -420,10 +420,10 @@ class Header(object):
         payload = bytearray(payload)
         mask = bytearray(self.mask)
 
-        for i in xrange(self.length):
+        for i in six.moves.range(self.length):
             payload[i] ^= mask[i % 4]
 
-        return str(payload)
+        return six.binary_type(payload)
 
     # it's the same operation
     unmask_payload = mask_payload
@@ -447,7 +447,7 @@ class Header(object):
         if len(data) != 2:
             raise WebSocketError("Unexpected EOF while decoding header")
 
-        first_byte, second_byte = struct.unpack('!BB', data)
+        first_byte, second_byte = struct.unpack(b'!BB', data)
 
         header = cls(
             fin=first_byte & cls.FIN_MASK == cls.FIN_MASK,
@@ -475,7 +475,7 @@ class Header(object):
             if len(data) != 2:
                 raise WebSocketError('Unexpected EOF while decoding header')
 
-            header.length = struct.unpack('!H', data)[0]
+            header.length = struct.unpack(b'!H', data)[0]
         elif header.length == 127:
             # 64 bit length
             data = read(8)
@@ -483,7 +483,7 @@ class Header(object):
             if len(data) != 8:
                 raise WebSocketError('Unexpected EOF while decoding header')
 
-            header.length = struct.unpack('!Q', data)[0]
+            header.length = struct.unpack(b'!Q', data)[0]
 
         if has_mask:
             mask = read(4)
@@ -509,7 +509,7 @@ class Header(object):
         """
         first_byte = opcode
         second_byte = 0
-        extra = ''
+        extra = b''
 
         if fin:
             first_byte |= cls.FIN_MASK
@@ -537,7 +537,6 @@ class Header(object):
 
         if mask:
             second_byte |= cls.MASK_MASK
-
             extra += mask
 
-        return chr(first_byte) + chr(second_byte) + extra
+        return six.b(chr(first_byte)) + six.b(chr(second_byte)) + extra
