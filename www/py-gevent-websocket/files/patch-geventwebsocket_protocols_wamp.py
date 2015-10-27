--- geventwebsocket/protocols/wamp.py.orig	2015-05-26 07:11:50 UTC
+++ geventwebsocket/protocols/wamp.py
@@ -1,5 +1,6 @@
 import inspect
 import random
+import six
 import string
 import types
 
@@ -168,9 +169,9 @@ class WampProtocol(BaseProtocol):
         call_id, curie_or_uri = data[1:3]
         args = data[3:]
 
-        if not isinstance(call_id, (str, unicode)):
+        if type(call_id) not in six.string_types:
             raise Exception()
-        if not isinstance(curie_or_uri, (str, unicode)):
+        if type(curie_or_uri) not in six.string_types:
             raise Exception()
 
         uri = self.prefixes.resolve(curie_or_uri)
@@ -178,7 +179,7 @@ class WampProtocol(BaseProtocol):
         try:
             result = self.procedures.call(uri, args)
             result_msg = [self.MSG_CALL_RESULT, call_id, result]
-        except Exception, e:
+        except Exception as e:
             result_msg = [self.MSG_CALL_ERROR,
                           call_id] + self._get_exception_info(e)
 
@@ -190,7 +191,7 @@ class WampProtocol(BaseProtocol):
 
         if not isinstance(action, int):
             raise Exception()
-        if not isinstance(curie_or_uri, (str, unicode)):
+        if type(curie_or_uri) not in six.string_types:
             raise Exception()
 
         uri = self.prefixes.resolve(curie_or_uri)
