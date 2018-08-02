--- dix/swapreq.c.orig	2016-07-15 16:18:11 UTC
+++ dix/swapreq.c
@@ -292,6 +292,13 @@ SProcSendEvent(ClientPtr client)
     swapl(&stuff->destination);
     swapl(&stuff->eventMask);
 
+    /* Generic events can have variable size, but SendEvent request holds
+       exactly 32B of event data. */
+    if (stuff->event.u.u.type == GenericEvent) {
+        client->errorValue = stuff->event.u.u.type;
+        return BadValue;
+    }
+
     /* Swap event */
     proc = EventSwapVector[stuff->event.u.u.type & 0177];
     if (!proc || proc == NotImplemented)        /* no swapping proc; invalid event type? */
