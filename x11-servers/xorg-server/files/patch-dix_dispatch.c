--- dix/dispatch.c.orig	2016-07-19 17:07:29 UTC
+++ dix/dispatch.c
@@ -3654,7 +3654,12 @@ ProcEstablishConnection(ClientPtr client)
     prefix = (xConnClientPrefix *) ((char *) stuff + sz_xReq);
     auth_proto = (char *) prefix + sz_xConnClientPrefix;
     auth_string = auth_proto + pad_to_int32(prefix->nbytesAuthProto);
-    if ((prefix->majorVersion != X_PROTOCOL) ||
+
+    if ((client->req_len << 2) != sz_xReq + sz_xConnClientPrefix +
+	pad_to_int32(prefix->nbytesAuthProto) +
+	pad_to_int32(prefix->nbytesAuthString))
+        reason = "Bad length";
+    else if ((prefix->majorVersion != X_PROTOCOL) ||
         (prefix->minorVersion != X_PROTOCOL_REVISION))
         reason = "Protocol version mismatch";
     else
