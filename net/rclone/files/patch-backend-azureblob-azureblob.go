diff --git backend/azureblob/azureblob.go backend/azureblob/azureblob.go
index 28d6d6ff..4b3df4c7 100644
--- backend/azureblob/azureblob.go
+++ backend/azureblob/azureblob.go
@@ -1,6 +1,6 @@
 // Package azureblob provides an interface to the Microsoft Azure blob object storage system
 
-// +build !freebsd,!netbsd,!openbsd,!plan9,!solaris,go1.8
+// +build go1.8
 
 package azureblob
