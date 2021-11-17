--- backend/onedrive/onedrive.go.orig
+++ backend/onedrive/onedrive.go
@@ -566,6 +566,9 @@ func shouldRetry(ctx context.Context, resp *http.Response, err error) (bool, err
                        if len(resp.Header["Www-Authenticate"]) == 1 && strings.Index(resp.Header["Www-Authenticate"][0], "expired_token") >= 0 {
                                retry = true
                                fs.Debugf(nil, "Should retry: %v", err)
+                       } else if err != nil && strings.Contains(err.Error(), "Unable to initialize RPS") {
+                               retry = true
+                               fs.Debugf(nil, "HTTP 401: Unable to initialize RPS. Trying again.")
                        }
                case 429: // Too Many Requests.
                        // see https://docs.microsoft.com/en-us/sharepoint/dev/general-development/how-to-avoid-getting-throttled-or-blocked-in-sharepoint-online
