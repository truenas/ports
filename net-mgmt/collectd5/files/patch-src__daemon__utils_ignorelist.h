--- src/daemon/utils_ignorelist.h	2017-01-23 07:53:57 UTC
+++ src/daemon/utils_ignorelist.g
@@ -61,6 +61,12 @@
 int ignorelist_add(ignorelist_t *il, const char *entry);
 
 /*
+ * remote entry from ignorelist_t
+ * returns zero on success, non-zero upon failure.
+ */
+int ignorelist_remove(ignorelist_t *il, const char *entry);
+
+/*
  * check list for entry
  * return 1 for ignored entry
  */
