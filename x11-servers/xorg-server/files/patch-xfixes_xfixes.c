--- xfixes/xfixes.c.orig	2016-07-19 17:07:29 UTC
+++ xfixes/xfixes.c
@@ -160,6 +160,7 @@ static int
 SProcXFixesQueryVersion(ClientPtr client)
 {
     REQUEST(xXFixesQueryVersionReq);
+    REQUEST_SIZE_MATCH(xXFixesQueryVersionReq);
 
     swaps(&stuff->length);
     swapl(&stuff->majorVersion);
