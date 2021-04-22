--- backend/b2/b2.go.orig
+++ backend/b2/b2.go
@@ -708,7 +708,7 @@ func (f *Fs) list(ctx context.Context, bucket, directory, prefix string, addBuck
 			remote := file.Name[len(prefix):]
 			// Check for directory
 			isDirectory := remote == "" || strings.HasSuffix(remote, "/")
-			if isDirectory {
+			if isDirectory && len(remote) > 1 {
 				remote = remote[:len(remote)-1]
 			}
 			if addBucket {
