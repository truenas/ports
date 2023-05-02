--- ws4py/streaming.py.orig	2018-02-28 16:24:28 UTC
+++ ws4py/streaming.py
@@ -225,9 +225,9 @@ class Stream(object):
                         self.message = m
 
                         if some_bytes:
-                            is_valid, end_on_code_point, _, _ = utf8validator.validate(some_bytes)
+                            is_valid = utf8validator.validate(some_bytes)
 
-                            if not is_valid or (m.completed and not end_on_code_point):
+                            if not is_valid:
                                 self.errors.append(CloseControlMessage(code=1007, reason='Invalid UTF-8 bytes'))
                                 break
 
@@ -252,9 +252,9 @@ class Stream(object):
                         m.completed = (frame.fin == 1)
                         if m.opcode == OPCODE_TEXT:
                             if some_bytes:
-                                is_valid, end_on_code_point, _, _ = utf8validator.validate(some_bytes)
+                                is_valid = utf8validator.validate(some_bytes)
 
-                                if not is_valid or (m.completed and not end_on_code_point):
+                                if not is_valid:
                                     self.errors.append(CloseControlMessage(code=1007, reason='Invalid UTF-8 bytes'))
                                     break
 
@@ -280,9 +280,11 @@ class Stream(object):
                                 elif frame.payload_length > 1:
                                     reason = some_bytes[2:] if frame.masking_key else frame.body[2:]
 
-                                    if not py3k: reason = bytearray(reason)
-                                    is_valid, end_on_code_point, _, _ = utf8validator.validate(reason)
-                                    if not is_valid or not end_on_code_point:
+                                    if not py3k:
+                                        reason = bytearray(reason)
+
+                                    is_valid = utf8validator.validate(reason)
+                                    if not is_valid:
                                         self.errors.append(CloseControlMessage(code=1007, reason='Invalid UTF-8 bytes'))
                                         break
                                     reason = bytes(reason)
@@ -309,11 +311,5 @@ class Stream(object):
             frame._cleanup()
             frame.body = None
             frame = None
-
-            if self.message is not None and self.message.completed:
-                utf8validator.reset()
-
-        utf8validator.reset()
-        utf8validator = None
 
         self._cleanup()
