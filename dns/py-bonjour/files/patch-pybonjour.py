--- pybonjour.py.orig	2017-12-05 12:16:44.128306000 -0500
+++ pybonjour.py	2017-12-05 12:16:57.803743000 -0500
@@ -1116,7 +1116,7 @@
             type=type(txtRecord)))
 
     if not txtRecord:
-        txtLen, txtRecord = 1, '\0'
+        txtLen, txtRecord = 0, 0
     else:
         txtLen, txtRecord = _string_to_length_and_void_p(txtRecord)
 
