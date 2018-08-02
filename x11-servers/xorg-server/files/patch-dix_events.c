--- dix/events.c.orig	2016-07-19 17:07:29 UTC
+++ dix/events.c
@@ -5355,6 +5355,12 @@ ProcSendEvent(ClientPtr client)
         client->errorValue = stuff->event.u.u.type;
         return BadValue;
     }
+    /* Generic events can have variable size, but SendEvent request holds
+       exactly 32B of event data. */
+    if (stuff->event.u.u.type == GenericEvent) {
+        client->errorValue = stuff->event.u.u.type;
+        return BadValue;
+    }
     if (stuff->event.u.u.type == ClientMessage &&
         stuff->event.u.u.detail != 8 &&
         stuff->event.u.u.detail != 16 && stuff->event.u.u.detail != 32) {
