--- nslcd/nslcd_json.h.orig	2019-08-20 10:11:08.677395931 -0700
+++ nslcd/nslcd_json.h	2019-08-20 10:22:25.352233598 -0700
@@ -0,0 +1,67 @@
+/*
+   Copied from lib/audit_logging/audit_logging.h in Samba
+
+   Copyright (C) Andrew Bartlett <abartlet@samba.org> 2018
+
+   This program is free software; you can redistribute it and/or modify
+   it under the terms of the GNU General Public License as published by
+   the Free Software Foundation; either version 3 of the License, or
+   (at your option) any later version.
+
+   This program is distributed in the hope that it will be useful,
+   but WITHOUT ANY WARRANTY; without even the implied warranty of
+   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
+   GNU General Public License for more details.
+
+   You should have received a copy of the GNU General Public License
+   along with this program.  If not, see <http://www.gnu.org/licenses/>.
+*/
+
+#include <stdio.h>
+#include <stdlib.h>
+#include <unistd.h>
+#include <string.h>
+#include <strings.h>
+#include <sys/time.h>
+#include <time.h>
+#include <sys/types.h>
+#include <sys/socket.h>
+#include <jansson.h>
+#include <stdbool.h>
+/*
+ * Wrapper for jannson JSON object
+ *
+ */
+struct json_object {
+        json_t *root;
+        bool error;
+};
+
+struct json_object json_new_object(void);
+void json_free(struct json_object *object);
+bool json_is_invalid(struct json_object *object);
+
+int json_add_int(struct json_object *object,
+                  const char* name,
+                  const int value);
+void json_add_bool(struct json_object *object,
+                   const char* name,
+                   const bool value);
+void json_add_string(struct json_object *object,
+                     const char* name,
+                     const char* value);
+void json_add_object(struct json_object *object,
+                     const char* name,
+                     struct json_object *value);
+void json_add_stringn(struct json_object *object,
+                      const char *name,
+                      const char *value,
+                      const size_t len);
+void json_add_timestamp(struct json_object *object);
+
+struct json_object json_get_array(struct json_object *object,
+                                  const char* name);
+struct json_object json_get_object(struct json_object *object,
+                                   const char* name);
+
+char *json_to_string(const struct json_object *object);
