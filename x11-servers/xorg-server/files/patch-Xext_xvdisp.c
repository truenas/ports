--- Xext/xvdisp.c.orig	2016-07-15 16:18:11 UTC
+++ Xext/xvdisp.c
@@ -1496,11 +1496,13 @@ XineramaXvShmPutImage(ClientPtr client)
 {
     REQUEST(xvShmPutImageReq);
     PanoramiXRes *draw, *gc, *port;
-    Bool send_event = stuff->send_event;
+    Bool send_event;
     Bool isRoot;
     int result, i, x, y;
 
     REQUEST_SIZE_MATCH(xvShmPutImageReq);
+
+    send_event = stuff->send_event;
 
     result = dixLookupResourceByClass((void **) &draw, stuff->drawable,
                                       XRC_DRAWABLE, client, DixWriteAccess);
