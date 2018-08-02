--- Xi/xibarriers.c.orig	2016-07-15 16:17:45 UTC
+++ Xi/xibarriers.c
@@ -830,10 +830,15 @@ SProcXIBarrierReleasePointer(ClientPtr client)
     REQUEST(xXIBarrierReleasePointerReq);
     int i;
 
-    info = (xXIBarrierReleasePointerInfo*) &stuff[1];
-
     swaps(&stuff->length);
+    REQUEST_AT_LEAST_SIZE(xXIBarrierReleasePointerReq);
+
     swapl(&stuff->num_barriers);
+    if (stuff->num_barriers > UINT32_MAX / sizeof(xXIBarrierReleasePointerInfo))
+        return BadLength;
+    REQUEST_FIXED_SIZE(xXIBarrierReleasePointerReq, stuff->num_barriers * sizeof(xXIBarrierReleasePointerInfo));
+
+    info = (xXIBarrierReleasePointerInfo*) &stuff[1];
     for (i = 0; i < stuff->num_barriers; i++, info++) {
         swaps(&info->deviceid);
         swapl(&info->barrier);
@@ -854,6 +859,10 @@ ProcXIBarrierReleasePointer(ClientPtr client)
 
     REQUEST(xXIBarrierReleasePointerReq);
     REQUEST_AT_LEAST_SIZE(xXIBarrierReleasePointerReq);
+    if (stuff->num_barriers > UINT32_MAX / sizeof(xXIBarrierReleasePointerInfo))
+        return BadLength;
+    REQUEST_FIXED_SIZE(xXIBarrierReleasePointerReq, stuff->num_barriers * sizeof(xXIBarrierReleasePointerInfo));
+
 
     info = (xXIBarrierReleasePointerInfo*) &stuff[1];
     for (i = 0; i < stuff->num_barriers; i++, info++) {
