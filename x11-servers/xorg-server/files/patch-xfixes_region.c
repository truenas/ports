--- xfixes/region.c.orig	2016-07-15 16:18:11 UTC
+++ xfixes/region.c
@@ -359,6 +359,7 @@ ProcXFixesCopyRegion(ClientPtr client)
     RegionPtr pSource, pDestination;
 
     REQUEST(xXFixesCopyRegionReq);
+    REQUEST_SIZE_MATCH(xXFixesCopyRegionReq);
 
     VERIFY_REGION(pSource, stuff->source, client, DixReadAccess);
     VERIFY_REGION(pDestination, stuff->destination, client, DixWriteAccess);
@@ -375,7 +376,7 @@ SProcXFixesCopyRegion(ClientPtr client)
     REQUEST(xXFixesCopyRegionReq);
 
     swaps(&stuff->length);
-    REQUEST_AT_LEAST_SIZE(xXFixesCopyRegionReq);
+    REQUEST_SIZE_MATCH(xXFixesCopyRegionReq);
     swapl(&stuff->source);
     swapl(&stuff->destination);
     return (*ProcXFixesVector[stuff->xfixesReqType]) (client);
