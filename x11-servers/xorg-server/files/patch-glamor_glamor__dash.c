--- glamor/glamor_dash.c.orig	2016-07-18 19:08:16 UTC
+++ glamor/glamor_dash.c
@@ -146,7 +146,7 @@ glamor_dash_setup(DrawablePtr drawable, GCPtr gc)
         goto bail;
 
     dash_pixmap = glamor_get_dash_pixmap(gc);
-    dash_priv = glamor_get_pixmap_private(pixmap);
+    dash_priv = glamor_get_pixmap_private(dash_pixmap);
 
     if (!GLAMOR_PIXMAP_PRIV_HAS_FBO(dash_priv))
         goto bail;
