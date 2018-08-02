--- dbe/dbe.c.orig	2016-07-15 16:18:11 UTC
+++ dbe/dbe.c
@@ -574,6 +574,9 @@ ProcDbeGetVisualInfo(ClientPtr client)
     XdbeScreenVisualInfo *pScrVisInfo;
 
     REQUEST_AT_LEAST_SIZE(xDbeGetVisualInfoReq);
+    if (stuff->n > UINT32_MAX / sizeof(CARD32))
+        return BadLength;
+    REQUEST_FIXED_SIZE(xDbeGetVisualInfoReq, stuff->n * sizeof(CARD32));
 
     if (stuff->n > UINT32_MAX / sizeof(DrawablePtr))
         return BadAlloc;
@@ -924,7 +927,7 @@ SProcDbeSwapBuffers(ClientPtr client)
 
     swapl(&stuff->n);
     if (stuff->n > UINT32_MAX / sizeof(DbeSwapInfoRec))
-        return BadAlloc;
+        return BadLength;
     REQUEST_FIXED_SIZE(xDbeSwapBuffersReq, stuff->n * sizeof(xDbeSwapInfo));
 
     if (stuff->n != 0) {
