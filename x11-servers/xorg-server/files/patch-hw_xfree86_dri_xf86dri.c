--- hw/xfree86/dri/xf86dri.c.orig	2016-07-15 16:18:11 UTC
+++ hw/xfree86/dri/xf86dri.c
@@ -570,6 +570,7 @@ static int
 SProcXF86DRIQueryDirectRenderingCapable(register ClientPtr client)
 {
     REQUEST(xXF86DRIQueryDirectRenderingCapableReq);
+    REQUEST_SIZE_MATCH(xXF86DRIQueryDirectRenderingCapableReq);
     swaps(&stuff->length);
     swapl(&stuff->screen);
     return ProcXF86DRIQueryDirectRenderingCapable(client);
