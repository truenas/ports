--- hw/dmx/dmxpict.c.orig	2016-07-15 16:18:11 UTC
+++ hw/dmx/dmxpict.c
@@ -716,6 +716,8 @@ dmxProcRenderSetPictureFilter(ClientPtr client)
         filter = (char *) (stuff + 1);
         params = (XFixed *) (filter + ((stuff->nbytes + 3) & ~3));
         nparams = ((XFixed *) stuff + client->req_len) - params;
+        if (nparams < 0)
+            return BadLength;
 
         XRenderSetPictureFilter(dmxScreen->beDisplay,
                                 pPictPriv->pict, filter, params, nparams);
