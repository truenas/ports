--- requests/__init__.py.orig	2019-11-29 23:53:17 UTC
+++ requests/__init__.py
@@ -57,10 +57,10 @@ def check_compatibility(urllib3_version, chardet_versi
     # Check urllib3 for compatibility.
     major, minor, patch = urllib3_version  # noqa: F811
     major, minor, patch = int(major), int(minor), int(patch)
-    # urllib3 >= 1.21.1, <= 1.24
+    # urllib3 >= 1.21.1, <= 1.25.6
     assert major == 1
     assert minor >= 21
-    assert minor <= 24
+    assert minor <= 25
 
     # Check chardet for compatibility.
     major, minor, patch = chardet_version.split('.')[:3]
