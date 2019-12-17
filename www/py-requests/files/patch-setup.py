--- setup.py.orig	2019-11-29 23:51:26 UTC
+++ setup.py
@@ -44,13 +44,12 @@ packages = ['requests']
 requires = [
     'chardet>=3.0.2,<3.1.0',
     'idna>=2.5,<2.9',
-    'urllib3>=1.21.1,<1.25',
+    'urllib3>=1.21.1,<1.26',
     'certifi>=2017.4.17'
 
 ]
 test_requirements = [
     'pytest-httpbin==0.0.7',
-    'pytest-cov',
     'pytest-mock',
     'pytest-xdist',
     'PySocks>=1.5.6, !=1.5.7',
