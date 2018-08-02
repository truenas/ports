--- Xext/panoramiX.c.orig	2016-07-15 16:18:11 UTC
+++ Xext/panoramiX.c
@@ -988,10 +988,11 @@ ProcPanoramiXGetScreenSize(ClientPtr client)
     xPanoramiXGetScreenSizeReply rep;
     int rc;
 
+    REQUEST_SIZE_MATCH(xPanoramiXGetScreenSizeReq);
+
     if (stuff->screen >= PanoramiXNumScreens)
         return BadMatch;
 
-    REQUEST_SIZE_MATCH(xPanoramiXGetScreenSizeReq);
     rc = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
     if (rc != Success)
         return rc;
