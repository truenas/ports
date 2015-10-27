--- geventwebsocket/utf8validator.py.orig	2013-04-10 19:05:54 UTC
+++ geventwebsocket/utf8validator.py
@@ -27,6 +27,7 @@
 ##
 ###############################################################################
 
+from six.moves import range
 
 ## use Cython implementation of UTF8 validator if available
 ##
@@ -113,11 +114,12 @@ except:
             """
 
             l = len(ba)
+            ba = bytearray(ba)
 
-            for i in xrange(l):
+            for i in range(l):
                 ## optimized version of decode(), since we are not interested in actual code points
 
-                self.state = Utf8Validator.UTF8VALIDATOR_DFA[256 + (self.state << 4) + Utf8Validator.UTF8VALIDATOR_DFA[ord(ba[i])]]
+                self.state = Utf8Validator.UTF8VALIDATOR_DFA[256 + (self.state << 4) + Utf8Validator.UTF8VALIDATOR_DFA[ba[i]]]
 
                 if self.state == Utf8Validator.UTF8_REJECT:
                     self.i += i
