--- backend/drive/drive.go.orig
+++ backend/drive/drive.go
@@ -17,7 +17,6 @@ import (
 	"log"
 	"mime"
 	"net/http"
-	"net/url"
 	"os"
 	"path"
 	"sort"
@@ -67,7 +66,7 @@ const (
 	// 1<<18 is the minimum size supported by the Google uploader, and there is no maximum.
 	minChunkSize     = 256 * fs.KibiByte
 	defaultChunkSize = 8 * fs.MebiByte
-	partialFields    = "id,name,size,md5Checksum,trashed,modifiedTime,createdTime,mimeType,parents,webViewLink"
+	partialFields    = "id,name,size,md5Checksum,trashed,modifiedTime,createdTime,mimeType,parents,webViewLink,exportLinks"
 )

 // Globals
@@ -1135,19 +1134,7 @@ func (f *Fs) newDocumentObject(remote string, info *drive.File, extension, expor
 	if err != nil {
 		return nil, err
 	}
-	url := fmt.Sprintf("%sfiles/%s/export?mimeType=%s", f.svc.BasePath, info.Id, url.QueryEscape(mediaType))
-	if f.opt.AlternateExport {
-		switch info.MimeType {
-		case "application/vnd.google-apps.drawing":
-			url = fmt.Sprintf("https://docs.google.com/drawings/d/%s/export/%s", info.Id, extension[1:])
-		case "application/vnd.google-apps.document":
-			url = fmt.Sprintf("https://docs.google.com/document/d/%s/export?format=%s", info.Id, extension[1:])
-		case "application/vnd.google-apps.spreadsheet":
-			url = fmt.Sprintf("https://docs.google.com/spreadsheets/d/%s/export?format=%s", info.Id, extension[1:])
-		case "application/vnd.google-apps.presentation":
-			url = fmt.Sprintf("https://docs.google.com/presentation/d/%s/export/%s", info.Id, extension[1:])
-		}
-	}
+	url := info.ExportLinks[mediaType]
 	baseObject := f.newBaseObject(remote+extension, info)
 	baseObject.bytes = -1
 	baseObject.mimeType = exportMimeType
