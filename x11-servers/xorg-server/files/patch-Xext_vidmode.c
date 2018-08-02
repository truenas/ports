--- Xext/vidmode.c.orig	2016-07-18 19:08:16 UTC
+++ Xext/vidmode.c
@@ -454,7 +454,21 @@ ProcVidModeAddModeLine(ClientPtr client)
     DEBUG_P("XF86VidModeAddModeline");
 
     ver = ClientMajorVersion(client);
+
     if (ver < 2) {
+        REQUEST_AT_LEAST_SIZE(xXF86OldVidModeAddModeLineReq);
+        len =
+            client->req_len -
+            bytes_to_int32(sizeof(xXF86OldVidModeAddModeLineReq));
+    }
+    else {
+        REQUEST_AT_LEAST_SIZE(xXF86VidModeAddModeLineReq);
+        len =
+            client->req_len -
+            bytes_to_int32(sizeof(xXF86VidModeAddModeLineReq));
+    }
+
+    if (ver < 2) {
         /* convert from old format */
         stuff = &newstuff;
         stuff->length = oldstuff->length;
@@ -501,18 +515,6 @@ ProcVidModeAddModeLine(ClientPtr client)
            stuff->after_vsyncend, stuff->after_vtotal,
            (unsigned long) stuff->after_flags);
 
-    if (ver < 2) {
-        REQUEST_AT_LEAST_SIZE(xXF86OldVidModeAddModeLineReq);
-        len =
-            client->req_len -
-            bytes_to_int32(sizeof(xXF86OldVidModeAddModeLineReq));
-    }
-    else {
-        REQUEST_AT_LEAST_SIZE(xXF86VidModeAddModeLineReq);
-        len =
-            client->req_len -
-            bytes_to_int32(sizeof(xXF86VidModeAddModeLineReq));
-    }
     if (len != stuff->privsize)
         return BadLength;
 
@@ -622,7 +624,21 @@ ProcVidModeDeleteModeLine(ClientPtr client)
     DEBUG_P("XF86VidModeDeleteModeline");
 
     ver = ClientMajorVersion(client);
+
     if (ver < 2) {
+        REQUEST_AT_LEAST_SIZE(xXF86OldVidModeDeleteModeLineReq);
+        len =
+            client->req_len -
+            bytes_to_int32(sizeof(xXF86OldVidModeDeleteModeLineReq));
+    }
+    else {
+        REQUEST_AT_LEAST_SIZE(xXF86VidModeDeleteModeLineReq);
+        len =
+            client->req_len -
+            bytes_to_int32(sizeof(xXF86VidModeDeleteModeLineReq));
+    }
+
+    if (ver < 2) {
         /* convert from old format */
         stuff = &newstuff;
         stuff->length = oldstuff->length;
@@ -649,18 +665,6 @@ ProcVidModeDeleteModeLine(ClientPtr client)
            stuff->vdisplay, stuff->vsyncstart, stuff->vsyncend, stuff->vtotal,
            (unsigned long) stuff->flags);
 
-    if (ver < 2) {
-        REQUEST_AT_LEAST_SIZE(xXF86OldVidModeDeleteModeLineReq);
-        len =
-            client->req_len -
-            bytes_to_int32(sizeof(xXF86OldVidModeDeleteModeLineReq));
-    }
-    else {
-        REQUEST_AT_LEAST_SIZE(xXF86VidModeDeleteModeLineReq);
-        len =
-            client->req_len -
-            bytes_to_int32(sizeof(xXF86VidModeDeleteModeLineReq));
-    }
     if (len != stuff->privsize) {
         DebugF("req_len = %ld, sizeof(Req) = %d, privsize = %ld, "
                "len = %d, length = %d\n",
@@ -744,7 +748,21 @@ ProcVidModeModModeLine(ClientPtr client)
     DEBUG_P("XF86VidModeModModeline");
 
     ver = ClientMajorVersion(client);
+
     if (ver < 2) {
+        REQUEST_AT_LEAST_SIZE(xXF86OldVidModeModModeLineReq);
+        len =
+            client->req_len -
+            bytes_to_int32(sizeof(xXF86OldVidModeModModeLineReq));
+    }
+    else {
+        REQUEST_AT_LEAST_SIZE(xXF86VidModeModModeLineReq);
+        len =
+            client->req_len -
+            bytes_to_int32(sizeof(xXF86VidModeModModeLineReq));
+    }
+
+    if (ver < 2) {
         /* convert from old format */
         stuff = &newstuff;
         stuff->length = oldstuff->length;
@@ -768,18 +786,6 @@ ProcVidModeModModeLine(ClientPtr client)
            stuff->vdisplay, stuff->vsyncstart, stuff->vsyncend,
            stuff->vtotal, (unsigned long) stuff->flags);
 
-    if (ver < 2) {
-        REQUEST_AT_LEAST_SIZE(xXF86OldVidModeModModeLineReq);
-        len =
-            client->req_len -
-            bytes_to_int32(sizeof(xXF86OldVidModeModModeLineReq));
-    }
-    else {
-        REQUEST_AT_LEAST_SIZE(xXF86VidModeModModeLineReq);
-        len =
-            client->req_len -
-            bytes_to_int32(sizeof(xXF86VidModeModModeLineReq));
-    }
     if (len != stuff->privsize)
         return BadLength;
 
@@ -877,7 +883,20 @@ ProcVidModeValidateModeLine(ClientPtr client)
     DEBUG_P("XF86VidModeValidateModeline");
 
     ver = ClientMajorVersion(client);
+
     if (ver < 2) {
+        REQUEST_AT_LEAST_SIZE(xXF86OldVidModeValidateModeLineReq);
+        len = client->req_len -
+            bytes_to_int32(sizeof(xXF86OldVidModeValidateModeLineReq));
+    }
+    else {
+        REQUEST_AT_LEAST_SIZE(xXF86VidModeValidateModeLineReq);
+        len =
+            client->req_len -
+            bytes_to_int32(sizeof(xXF86VidModeValidateModeLineReq));
+    }
+
+    if (ver < 2) {
         /* convert from old format */
         stuff = &newstuff;
         stuff->length = oldstuff->length;
@@ -905,17 +924,6 @@ ProcVidModeValidateModeLine(ClientPtr client)
            stuff->vdisplay, stuff->vsyncstart, stuff->vsyncend, stuff->vtotal,
            (unsigned long) stuff->flags);
 
-    if (ver < 2) {
-        REQUEST_AT_LEAST_SIZE(xXF86OldVidModeValidateModeLineReq);
-        len = client->req_len -
-            bytes_to_int32(sizeof(xXF86OldVidModeValidateModeLineReq));
-    }
-    else {
-        REQUEST_AT_LEAST_SIZE(xXF86VidModeValidateModeLineReq);
-        len =
-            client->req_len -
-            bytes_to_int32(sizeof(xXF86VidModeValidateModeLineReq));
-    }
     if (len != stuff->privsize)
         return BadLength;
 
@@ -1027,7 +1035,21 @@ ProcVidModeSwitchToMode(ClientPtr client)
     DEBUG_P("XF86VidModeSwitchToMode");
 
     ver = ClientMajorVersion(client);
+
     if (ver < 2) {
+        REQUEST_AT_LEAST_SIZE(xXF86OldVidModeSwitchToModeReq);
+        len =
+            client->req_len -
+            bytes_to_int32(sizeof(xXF86OldVidModeSwitchToModeReq));
+    }
+    else {
+        REQUEST_AT_LEAST_SIZE(xXF86VidModeSwitchToModeReq);
+        len =
+            client->req_len -
+            bytes_to_int32(sizeof(xXF86VidModeSwitchToModeReq));
+    }
+
+    if (ver < 2) {
         /* convert from old format */
         stuff = &newstuff;
         stuff->length = oldstuff->length;
@@ -1055,18 +1077,6 @@ ProcVidModeSwitchToMode(ClientPtr client)
            stuff->vdisplay, stuff->vsyncstart, stuff->vsyncend, stuff->vtotal,
            (unsigned long) stuff->flags);
 
-    if (ver < 2) {
-        REQUEST_AT_LEAST_SIZE(xXF86OldVidModeSwitchToModeReq);
-        len =
-            client->req_len -
-            bytes_to_int32(sizeof(xXF86OldVidModeSwitchToModeReq));
-    }
-    else {
-        REQUEST_AT_LEAST_SIZE(xXF86VidModeSwitchToModeReq);
-        len =
-            client->req_len -
-            bytes_to_int32(sizeof(xXF86VidModeSwitchToModeReq));
-    }
     if (len != stuff->privsize)
         return BadLength;
 
@@ -1457,6 +1467,7 @@ ProcVidModeSetGammaRamp(ClientPtr client)
     VidModePtr pVidMode;
 
     REQUEST(xXF86VidModeSetGammaRampReq);
+    REQUEST_AT_LEAST_SIZE(xXF86VidModeSetGammaRampReq);
 
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
