--- hw/xfree86/common/xf86DGA.c.orig	2016-07-15 16:18:11 UTC
+++ hw/xfree86/common/xf86DGA.c
@@ -1272,13 +1272,14 @@ ProcXDGAOpenFramebuffer(ClientPtr client)
     char *deviceName;
     int nameSize;
 
+    REQUEST_SIZE_MATCH(xXDGAOpenFramebufferReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (!DGAAvailable(stuff->screen))
         return DGAErrorBase + XF86DGANoDirectVideoMode;
 
-    REQUEST_SIZE_MATCH(xXDGAOpenFramebufferReq);
     rep.type = X_Reply;
     rep.length = 0;
     rep.sequenceNumber = client->sequence;
@@ -1305,14 +1306,14 @@ ProcXDGACloseFramebuffer(ClientPtr client)
 {
     REQUEST(xXDGACloseFramebufferReq);
 
+    REQUEST_SIZE_MATCH(xXDGACloseFramebufferReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (!DGAAvailable(stuff->screen))
         return DGAErrorBase + XF86DGANoDirectVideoMode;
 
-    REQUEST_SIZE_MATCH(xXDGACloseFramebufferReq);
-
     DGACloseFramebuffer(stuff->screen);
 
     return Success;
@@ -1328,10 +1329,11 @@ ProcXDGAQueryModes(ClientPtr client)
     xXDGAModeInfo info;
     XDGAModePtr mode;
 
+    REQUEST_SIZE_MATCH(xXDGAQueryModesReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
-    REQUEST_SIZE_MATCH(xXDGAQueryModesReq);
     rep.type = X_Reply;
     rep.length = 0;
     rep.number = 0;
@@ -1443,11 +1445,12 @@ ProcXDGASetMode(ClientPtr client)
     ClientPtr owner;
     int size;
 
+    REQUEST_SIZE_MATCH(xXDGASetModeReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
     owner = DGA_GETCLIENT(stuff->screen);
 
-    REQUEST_SIZE_MATCH(xXDGASetModeReq);
     rep.type = X_Reply;
     rep.length = 0;
     rep.offset = 0;
@@ -1533,14 +1536,14 @@ ProcXDGASetViewport(ClientPtr client)
 {
     REQUEST(xXDGASetViewportReq);
 
+    REQUEST_SIZE_MATCH(xXDGASetViewportReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
 
-    REQUEST_SIZE_MATCH(xXDGASetViewportReq);
-
     DGASetViewport(stuff->screen, stuff->x, stuff->y, stuff->flags);
 
     return Success;
@@ -1554,14 +1557,14 @@ ProcXDGAInstallColormap(ClientPtr client)
 
     REQUEST(xXDGAInstallColormapReq);
 
+    REQUEST_SIZE_MATCH(xXDGAInstallColormapReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
 
-    REQUEST_SIZE_MATCH(xXDGAInstallColormapReq);
-
     rc = dixLookupResourceByType((void **) &cmap, stuff->cmap, RT_COLORMAP,
                                  client, DixInstallAccess);
     if (rc != Success)
@@ -1575,14 +1578,14 @@ ProcXDGASelectInput(ClientPtr client)
 {
     REQUEST(xXDGASelectInputReq);
 
+    REQUEST_SIZE_MATCH(xXDGASelectInputReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
 
-    REQUEST_SIZE_MATCH(xXDGASelectInputReq);
-
     if (DGA_GETCLIENT(stuff->screen) == client)
         DGASelectInput(stuff->screen, client, stuff->mask);
 
@@ -1594,14 +1597,14 @@ ProcXDGAFillRectangle(ClientPtr client)
 {
     REQUEST(xXDGAFillRectangleReq);
 
+    REQUEST_SIZE_MATCH(xXDGAFillRectangleReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
 
-    REQUEST_SIZE_MATCH(xXDGAFillRectangleReq);
-
     if (Success != DGAFillRect(stuff->screen, stuff->x, stuff->y,
                                stuff->width, stuff->height, stuff->color))
         return BadMatch;
@@ -1614,14 +1617,14 @@ ProcXDGACopyArea(ClientPtr client)
 {
     REQUEST(xXDGACopyAreaReq);
 
+    REQUEST_SIZE_MATCH(xXDGACopyAreaReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
 
-    REQUEST_SIZE_MATCH(xXDGACopyAreaReq);
-
     if (Success != DGABlitRect(stuff->screen, stuff->srcx, stuff->srcy,
                                stuff->width, stuff->height, stuff->dstx,
                                stuff->dsty))
@@ -1635,14 +1638,14 @@ ProcXDGACopyTransparentArea(ClientPtr client)
 {
     REQUEST(xXDGACopyTransparentAreaReq);
 
+    REQUEST_SIZE_MATCH(xXDGACopyTransparentAreaReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
 
-    REQUEST_SIZE_MATCH(xXDGACopyTransparentAreaReq);
-
     if (Success != DGABlitTransRect(stuff->screen, stuff->srcx, stuff->srcy,
                                     stuff->width, stuff->height, stuff->dstx,
                                     stuff->dsty, stuff->key))
@@ -1657,13 +1660,14 @@ ProcXDGAGetViewportStatus(ClientPtr client)
     REQUEST(xXDGAGetViewportStatusReq);
     xXDGAGetViewportStatusReply rep;
 
+    REQUEST_SIZE_MATCH(xXDGAGetViewportStatusReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
 
-    REQUEST_SIZE_MATCH(xXDGAGetViewportStatusReq);
     rep.type = X_Reply;
     rep.length = 0;
     rep.sequenceNumber = client->sequence;
@@ -1680,13 +1684,14 @@ ProcXDGASync(ClientPtr client)
     REQUEST(xXDGASyncReq);
     xXDGASyncReply rep;
 
+    REQUEST_SIZE_MATCH(xXDGASyncReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
 
-    REQUEST_SIZE_MATCH(xXDGASyncReq);
     rep.type = X_Reply;
     rep.length = 0;
     rep.sequenceNumber = client->sequence;
@@ -1725,13 +1730,14 @@ ProcXDGAChangePixmapMode(ClientPtr client)
     xXDGAChangePixmapModeReply rep;
     int x, y;
 
+    REQUEST_SIZE_MATCH(xXDGAChangePixmapModeReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
 
-    REQUEST_SIZE_MATCH(xXDGAChangePixmapModeReq);
     rep.type = X_Reply;
     rep.length = 0;
     rep.sequenceNumber = client->sequence;
@@ -1755,14 +1761,14 @@ ProcXDGACreateColormap(ClientPtr client)
     REQUEST(xXDGACreateColormapReq);
     int result;
 
+    REQUEST_SIZE_MATCH(xXDGACreateColormapReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
 
-    REQUEST_SIZE_MATCH(xXDGACreateColormapReq);
-
     if (!stuff->mode)
         return BadValue;
 
@@ -1791,10 +1797,11 @@ ProcXF86DGAGetVideoLL(ClientPtr client)
     int num, offset, flags;
     char *name;
 
+    REQUEST_SIZE_MATCH(xXF86DGAGetVideoLLReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
-    REQUEST_SIZE_MATCH(xXF86DGAGetVideoLLReq);
     rep.type = X_Reply;
     rep.length = 0;
     rep.sequenceNumber = client->sequence;
@@ -1831,9 +1838,10 @@ ProcXF86DGADirectVideo(ClientPtr client)
 
     REQUEST(xXF86DGADirectVideoReq);
 
+    REQUEST_SIZE_MATCH(xXF86DGADirectVideoReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
-    REQUEST_SIZE_MATCH(xXF86DGADirectVideoReq);
 
     if (!DGAAvailable(stuff->screen))
         return DGAErrorBase + XF86DGANoDirectVideoMode;
@@ -1889,10 +1897,11 @@ ProcXF86DGAGetViewPortSize(ClientPtr client)
     REQUEST(xXF86DGAGetViewPortSizeReq);
     xXF86DGAGetViewPortSizeReply rep;
 
+    REQUEST_SIZE_MATCH(xXF86DGAGetViewPortSizeReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
-    REQUEST_SIZE_MATCH(xXF86DGAGetViewPortSizeReq);
     rep.type = X_Reply;
     rep.length = 0;
     rep.sequenceNumber = client->sequence;
@@ -1917,14 +1926,14 @@ ProcXF86DGASetViewPort(ClientPtr client)
 {
     REQUEST(xXF86DGASetViewPortReq);
 
+    REQUEST_SIZE_MATCH(xXF86DGASetViewPortReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
 
-    REQUEST_SIZE_MATCH(xXF86DGASetViewPortReq);
-
     if (!DGAAvailable(stuff->screen))
         return DGAErrorBase + XF86DGANoDirectVideoMode;
 
@@ -1944,10 +1953,11 @@ ProcXF86DGAGetVidPage(ClientPtr client)
     REQUEST(xXF86DGAGetVidPageReq);
     xXF86DGAGetVidPageReply rep;
 
+    REQUEST_SIZE_MATCH(xXF86DGAGetVidPageReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
-    REQUEST_SIZE_MATCH(xXF86DGAGetVidPageReq);
     rep.type = X_Reply;
     rep.length = 0;
     rep.sequenceNumber = client->sequence;
@@ -1962,11 +1972,11 @@ ProcXF86DGASetVidPage(ClientPtr client)
 {
     REQUEST(xXF86DGASetVidPageReq);
 
+    REQUEST_SIZE_MATCH(xXF86DGASetVidPageReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
-    REQUEST_SIZE_MATCH(xXF86DGASetVidPageReq);
-
     /* silently fail */
 
     return Success;
@@ -1980,14 +1990,14 @@ ProcXF86DGAInstallColormap(ClientPtr client)
 
     REQUEST(xXF86DGAInstallColormapReq);
 
+    REQUEST_SIZE_MATCH(xXF86DGAInstallColormapReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
 
-    REQUEST_SIZE_MATCH(xXF86DGAInstallColormapReq);
-
     if (!DGAActive(stuff->screen))
         return DGAErrorBase + XF86DGADirectNotActivated;
 
@@ -2008,10 +2018,11 @@ ProcXF86DGAQueryDirectVideo(ClientPtr client)
     REQUEST(xXF86DGAQueryDirectVideoReq);
     xXF86DGAQueryDirectVideoReply rep;
 
+    REQUEST_SIZE_MATCH(xXF86DGAQueryDirectVideoReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
-    REQUEST_SIZE_MATCH(xXF86DGAQueryDirectVideoReq);
     rep.type = X_Reply;
     rep.length = 0;
     rep.sequenceNumber = client->sequence;
@@ -2030,13 +2041,13 @@ ProcXF86DGAViewPortChanged(ClientPtr client)
     REQUEST(xXF86DGAViewPortChangedReq);
     xXF86DGAViewPortChangedReply rep;
 
+    REQUEST_SIZE_MATCH(xXF86DGAViewPortChangedReq);
+
     if (stuff->screen >= screenInfo.numScreens)
         return BadValue;
 
     if (DGA_GETCLIENT(stuff->screen) != client)
         return DGAErrorBase + XF86DGADirectNotActivated;
-
-    REQUEST_SIZE_MATCH(xXF86DGAViewPortChangedReq);
 
     if (!DGAActive(stuff->screen))
         return DGAErrorBase + XF86DGADirectNotActivated;
