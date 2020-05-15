--- include/atalk/util.h.orig	2020-05-15 08:33:12.990075845 -0700
+++ include/atalk/util.h	2020-05-15 08:33:24.962295788 -0700
@@ -224,4 +224,27 @@ extern cnid_t cnid_for_path(struct _cnid_db *cdb, cons
 extern void initline   (int, char *);
 extern int  parseline  (int, char *);
 
+/**
+ * @brief Create a PID file
+ *
+ * Leave PID file open/locked on success, exit on failure.  On
+ * success, use pidfile_unlink() to remove PID file before exiting.
+ *
+ * PID file name is <piddir>/<name>.pid
+ *
+ * @param[in] piddir Directory for PID file
+ * @param[in] name PID file process name
+ */
+extern void pidfile_create(const char *piddir, const char *name);
+
+/**
+ * @brief Remove a PID file
+ *
+ * PID file name is <piddir>/<name>.pid
+ *
+ * @param[in] piddir Directory for PID file
+ * @param[in] name PID file process name
+ */
+extern void pidfile_unlink(const char *piddir, const char *name);
+
 #endif  /* _ATALK_UTIL_H */
