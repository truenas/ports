--- src/rrd_xport.c.orig	2016-01-27 10:49:53 UTC
+++ src/rrd_xport.c
@@ -265,7 +265,7 @@ static int rrd_xport_fn(
 
 
     /* pull the data from the rrd files ... */
-    if (data_fetch(im) == -1)
+    if (data_fetch(im) != 0)
         return -1;
 
     /* evaluate CDEF  operations ... */
