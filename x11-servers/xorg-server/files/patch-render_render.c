--- render/render.c.orig	2016-07-19 17:07:29 UTC
+++ render/render.c
@@ -1770,6 +1770,9 @@ ProcRenderSetPictureFilter(ClientPtr client)
     name = (char *) (stuff + 1);
     params = (xFixed *) (name + pad_to_int32(stuff->nbytes));
     nparams = ((xFixed *) stuff + client->req_len) - params;
+    if (nparams < 0)
+	return BadLength;
+
     result = SetPictureFilter(pPicture, name, stuff->nbytes, params, nparams);
     return result;
 }
