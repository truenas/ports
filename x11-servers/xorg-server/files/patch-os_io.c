--- os/io.c.orig	2016-07-19 17:07:29 UTC
+++ os/io.c
@@ -480,6 +480,11 @@ ReadRequestFromClient(ClientPtr client)
         if (++timesThisConnection >= MAX_TIMES_PER)
             YieldControl();
     if (move_header) {
+        if (client->req_len < bytes_to_int32(sizeof(xBigReq) - sizeof(xReq))) {
+            YieldControlDeath();
+            return -1;
+        }
+
         request = (xReq *) oci->bufptr;
         oci->bufptr += (sizeof(xBigReq) - sizeof(xReq));
         *(xReq *) oci->bufptr = *request;
