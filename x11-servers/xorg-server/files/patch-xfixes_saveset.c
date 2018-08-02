--- xfixes/saveset.c.orig	2016-07-15 16:17:30 UTC
+++ xfixes/saveset.c
@@ -62,6 +62,7 @@ int
 SProcXFixesChangeSaveSet(ClientPtr client)
 {
     REQUEST(xXFixesChangeSaveSetReq);
+    REQUEST_SIZE_MATCH(xXFixesChangeSaveSetReq);
 
     swaps(&stuff->length);
     swapl(&stuff->window);
