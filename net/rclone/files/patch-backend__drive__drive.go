--- backend/drive/drive.go.orig
+++ backend/drive/drive.go
@@ -593,7 +593,7 @@ func containsString(slice []string, s string) bool {
 func (f *Fs) getRootID() (string, error) {
 	var info *drive.File
 	var err error
-	err = f.pacer.CallNoRetry(func() (bool, error) {
+	err = f.pacer.Call(func() (bool, error) {
 		info, err = f.svc.Files.Get("root").
 			Fields("id").
 			SupportsAllDrives(true).
