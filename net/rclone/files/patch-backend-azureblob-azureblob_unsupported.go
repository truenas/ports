diff --git backend/azureblob/azureblob_unsupported.go backend/azureblob/azureblob_unsupported.go
index 54947980..4fabb479 100644
--- backend/azureblob/azureblob_unsupported.go
+++ backend/azureblob/azureblob_unsupported.go
@@ -1,6 +1,6 @@
 // Build for azureblob for unsupported platforms to stop go complaining
 // about "no buildable Go source files "
 
-// +build freebsd netbsd openbsd plan9 solaris !go1.8
+// +build !go1.8
 
 package azureblob
