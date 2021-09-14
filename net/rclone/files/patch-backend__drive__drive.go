--- backend/drive/drive.go.orig
+++ backend/drive/drive.go
@@ -1321,8 +1321,8 @@ func (f *Fs) newLinkObject(remote string, info *drive.File, extension, exportMim
 //
 // When the drive.File cannot be represented as an fs.Object it will return (nil, nil).
 func (f *Fs) newObjectWithInfo(remote string, info *drive.File) (fs.Object, error) {
-	// If item has MD5 sum or a length it is a file stored on drive
-	if info.Md5Checksum != "" || info.Size > 0 {
+	// If item has MD5 sum it is a file stored on drive
+	if info.Md5Checksum != "" {
 		return f.newRegularObject(remote, info), nil
 	}
 
@@ -1355,8 +1355,8 @@ func (f *Fs) newObjectWithExportInfo(
 		// Pretend a dangling shortcut is a regular object
 		// It will error if used, but appear in listings so it can be deleted
 		return f.newRegularObject(remote, info), nil
-	case info.Md5Checksum != "" || info.Size > 0:
-		// If item has MD5 sum or a length it is a file stored on drive
+	case info.Md5Checksum != "":
+		// If item has MD5 sum it is a file stored on drive
 		return f.newRegularObject(remote, info), nil
 	case f.opt.SkipGdocs:
 		fs.Debugf(remote, "Skipping google document type %q", info.MimeType)
