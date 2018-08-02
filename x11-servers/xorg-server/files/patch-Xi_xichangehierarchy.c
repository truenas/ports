--- Xi/xichangehierarchy.c.orig	2016-07-15 16:18:11 UTC
+++ Xi/xichangehierarchy.c
@@ -421,7 +421,7 @@ ProcXIChangeHierarchy(ClientPtr client)
     if (!stuff->num_changes)
         return rc;
 
-    len = ((size_t)stuff->length << 2) - sizeof(xXIAnyHierarchyChangeInfo);
+    len = ((size_t)stuff->length << 2) - sizeof(xXIChangeHierarchyReq);
 
     any = (xXIAnyHierarchyChangeInfo *) &stuff[1];
     while (stuff->num_changes--) {
