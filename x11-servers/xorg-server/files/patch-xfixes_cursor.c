--- xfixes/cursor.c.orig	2016-07-19 17:07:29 UTC
+++ xfixes/cursor.c
@@ -280,6 +280,7 @@ int
 SProcXFixesSelectCursorInput(ClientPtr client)
 {
     REQUEST(xXFixesSelectCursorInputReq);
+    REQUEST_SIZE_MATCH(xXFixesSelectCursorInputReq);
 
     swaps(&stuff->length);
     swapl(&stuff->window);
@@ -413,7 +414,7 @@ ProcXFixesSetCursorName(ClientPtr client)
     REQUEST(xXFixesSetCursorNameReq);
     Atom atom;
 
-    REQUEST_AT_LEAST_SIZE(xXFixesSetCursorNameReq);
+    REQUEST_FIXED_SIZE(xXFixesSetCursorNameReq, stuff->nbytes);
     VERIFY_CURSOR(pCursor, stuff->cursor, client, DixSetAttrAccess);
     tchar = (char *) &stuff[1];
     atom = MakeAtom(tchar, stuff->nbytes, TRUE);
@@ -1005,6 +1006,8 @@ SProcXFixesCreatePointerBarrier(ClientPtr client)
     REQUEST(xXFixesCreatePointerBarrierReq);
     int i;
     CARD16 *in_devices = (CARD16 *) &stuff[1];
+
+    REQUEST_AT_LEAST_SIZE(xXFixesCreatePointerBarrierReq);
 
     swaps(&stuff->length);
     swaps(&stuff->num_devices);
