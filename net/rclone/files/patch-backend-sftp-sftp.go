diff --git backend/sftp/sftp.go backend/sftp/sftp.go
index 1c4b75767..72f103fa1 100644
--- backend/sftp/sftp.go
+++ backend/sftp/sftp.go
@@ -565,9 +565,13 @@ func (f *Fs) List(dir string) (entries fs.DirEntries, err error) {
 		// If file is a symlink (not a regular file is the best cross platform test we can do), do a stat to
 		// pick up the size and type of the destination, instead of the size and type of the symlink.
 		if !info.Mode().IsRegular() {
+			oldInfo := info
 			info, err = f.stat(remote)
 			if err != nil {
-				return nil, errors.Wrap(err, "stat of non-regular file/dir failed")
+				if !os.IsNotExist(err) {
+					fs.Errorf(remote, "stat of non-regular file/dir failed: %v", err)
+				}
+				info = oldInfo
 			}
 		}
 		if info.IsDir() {
