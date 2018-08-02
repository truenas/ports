--- Xext/xres.c.orig	2016-07-19 17:07:29 UTC
+++ Xext/xres.c
@@ -1039,6 +1039,8 @@ ProcXResQueryResourceBytes (ClientPtr client)
     ConstructResourceBytesCtx    ctx;
 
     REQUEST_AT_LEAST_SIZE(xXResQueryResourceBytesReq);
+    if (stuff->numSpecs > UINT32_MAX / sizeof(ctx.specs[0]))
+        return BadLength;
     REQUEST_FIXED_SIZE(xXResQueryResourceBytesReq,
                        stuff->numSpecs * sizeof(ctx.specs[0]));
 
@@ -1144,8 +1146,8 @@ SProcXResQueryResourceBytes (ClientPtr client)
     int c;
     xXResResourceIdSpec *specs = (void*) ((char*) stuff + sizeof(*stuff));
 
-    swapl(&stuff->numSpecs);
     REQUEST_AT_LEAST_SIZE(xXResQueryResourceBytesReq);
+    swapl(&stuff->numSpecs);
     REQUEST_FIXED_SIZE(xXResQueryResourceBytesReq,
                        stuff->numSpecs * sizeof(specs[0]));
 
