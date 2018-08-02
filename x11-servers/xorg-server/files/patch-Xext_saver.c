--- Xext/saver.c.orig	2016-07-18 19:08:16 UTC
+++ Xext/saver.c
@@ -1185,6 +1185,8 @@ ProcScreenSaverUnsetAttributes(ClientPtr client)
         PanoramiXRes *draw;
         int rc, i;
 
+        REQUEST_SIZE_MATCH(xScreenSaverUnsetAttributesReq);
+
         rc = dixLookupResourceByClass((void **) &draw, stuff->drawable,
                                       XRC_DRAWABLE, client, DixWriteAccess);
         if (rc != Success)
