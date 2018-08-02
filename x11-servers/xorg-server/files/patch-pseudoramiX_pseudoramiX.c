--- pseudoramiX/pseudoramiX.c.orig	2016-07-15 16:18:11 UTC
+++ pseudoramiX/pseudoramiX.c
@@ -297,10 +297,11 @@ ProcPseudoramiXGetScreenSize(ClientPtr client)
 
     TRACE;
 
+    REQUEST_SIZE_MATCH(xPanoramiXGetScreenSizeReq);
+
     if (stuff->screen >= pseudoramiXNumScreens)
       return BadMatch;
 
-    REQUEST_SIZE_MATCH(xPanoramiXGetScreenSizeReq);
     rc = dixLookupWindow(&pWin, stuff->window, client, DixGetAttrAccess);
     if (rc != Success)
         return rc;
