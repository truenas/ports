--- setup.py.orig	2014-02-19 10:21:32 UTC
+++ setup.py
@@ -16,7 +16,7 @@ setup(
     packages=find_packages(exclude=["examples", "tests"]),
     license=open('LICENSE').read(),
     zip_safe=False,
-    install_requires=("gevent"),
+    install_requires=("gevent", "six"),
     classifiers=[
         "Environment :: Web Environment",
         "Intended Audience :: Developers",
