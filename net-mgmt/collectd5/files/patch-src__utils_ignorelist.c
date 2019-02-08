--- src/utils_ignorelist.c	2017-01-23 07:53:57 UTC
+++ src/utils_ignorelist.c
@@ -278,6 +278,40 @@ int ignorelist_add(ignorelist_t *il, const char *entry) {
   return ignorelist_append_string(il, entry);
 } /* int ignorelist_add (ignorelist_t *il, const char *entry) */
 
+/*
+ * remove entry from ignorelist_t
+ * return 0 for success
+ */
+int ignorelist_remove(ignorelist_t *il, const char *entry) {
+  /* if no entries, nothing to remove */
+  if ((il == NULL) || (il->head == NULL))
+    return (1);
+
+  if ((entry == NULL) || (strlen(entry) == 0))
+    return (1);
+
+  /* traverse list and check entries */
+  for (ignorelist_item_t *prev = NULL, *traverse = il->head; traverse != NULL;
+       prev = traverse, traverse = traverse->next) {
+    if (traverse->smatch != NULL && strcmp(traverse->smatch, entry) == 0) {
+      if (prev == NULL)
+      {
+        il->head = traverse->next;
+      }
+      else
+      {
+        prev->next = traverse->next;
+      }
+      sfree(traverse->smatch);
+      traverse->smatch = NULL;
+      sfree(traverse);
+      return (0);
+    }
+  } /* for traverse */
+
+  return (1);
+} /* int ignorelist_remove (ignorelist_t *il, const char *entry) */
+
 /*
  * check list for entry
  * return 1 for ignored entry
