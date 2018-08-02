--- Xext/shm.c.orig	2016-07-19 17:14:21 UTC
+++ Xext/shm.c
@@ -1238,6 +1238,7 @@ ProcShmCreateSegment(ClientPtr client)
     };
 
     REQUEST_SIZE_MATCH(xShmCreateSegmentReq);
+    LEGAL_NEW_RESOURCE(stuff->shmseg, client);
     if ((stuff->readOnly != xTrue) && (stuff->readOnly != xFalse)) {
         client->errorValue = stuff->readOnly;
         return BadValue;
