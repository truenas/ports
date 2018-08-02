--- Xi/sendexev.c.orig	2018-08-02 13:25:10 UTC
+++ Xi/sendexev.c
@@ -78,7 +78,7 @@ SProcXSendExtensionEvent(ClientPtr client)
 {
     CARD32 *p;
     int i;
-    xEvent eventT;
+    xEvent eventT = { .u.u.type = 0 };
     xEvent *eventP;
     EventSwapPtr proc;
 
