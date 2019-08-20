--- nslcd/nslcd_json.c.orig	2019-08-20 10:23:43.234567591 -0700
+++ nslcd/nslcd_json.c	2019-08-20 10:25:42.287273042 -0700
@@ -0,0 +1,223 @@
+/*
+   Copied from lib/audit_logging/audit_logging.c in Samba 
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
+#include "config.h"
+
+#include <stdio.h>
+#include <stdlib.h>
+#include <errno.h>
+#include <unistd.h>
+#include <string.h>
+#include <strings.h>
+#include <sys/time.h>
+#include <time.h>
+#include <sys/types.h>
+#include <sys/socket.h>
+#include "nslcd/nslcd_json.h"
+#include "nslcd/log.h"
+#include <jansson.h>
+
+
+struct json_object json_new_object(void) {
+
+        struct json_object object;
+        object.error = false;
+ 
+        object.root = json_object();
+        if (object.root == NULL) {
+                object.error = true;
+                log_log(LOG_WARNING,"Unable to create json_object\n");
+        }
+        return object;
+}
+    
+void json_free(struct json_object *object)
+{
+        if (object->root != NULL) {
+                json_decref(object->root);
+        }
+        object->root = NULL;
+        object->error = true;
+}
+  
+bool json_is_invalid(struct json_object *object)
+{
+        return object->error;
+}
+  
+int json_add_int(struct json_object *object,
+                  const char* name,
+                  const int value)
+{
+        int rc = 0;
+    
+        if (object->error) {
+                return -1;
+        }
+    
+        rc = json_object_set_new(object->root, name, json_integer(value));
+        if (rc) {
+                return -1;
+                log_log(LOG_WARNING, "Unable to set name [%s] value [%d]\n", name, value);
+                object->error = true;
+        }
+        return 0;
+}
+    
+void json_add_bool(struct json_object *object,
+                   const char* name,
+                   const bool value)
+{
+        int rc = 0;
+
+        if (object->error) {
+                return;
+        }   
+    
+        rc = json_object_set_new(object->root, name, json_boolean(value));
+        if (rc) {
+                log_log(LOG_WARNING, "Unable to set name [%s] value [%d]\n", name, value);
+                object->error = true;
+        }
+   
+}
+                         
+void json_add_string(struct json_object *object,
+                     const char* name,
+                     const char* value)
+{
+        int rc = 0;
+    
+        if (object->error) {
+                return;
+        }
+  
+        if (value) {
+                rc = json_object_set_new(
+                        object->root,
+                        name,
+                        json_string(value));
+        } else {
+                rc = json_object_set_new(object->root, name, json_null());
+        }
+        if (rc) {
+                log_log(LOG_WARNING, "Unable to set name [%s] value [%s]\n", name, value);
+                object->error = true;
+        }
+}
+
+void json_add_timestamp(struct json_object *object)
+{
+        char buffer[40];        /* formatted time less usec and timezone */
+        char timestamp[65];     /* the formatted ISO 8601 time stamp     */
+        char tz[10];            /* formatted time zone                   */
+        struct tm* tm_info;     /* current local time                    */
+        struct timeval tv;      /* current system time                   */
+        int r;                  /* response code from gettimeofday       */
+
+        if (object->error) {
+                return;
+        }
+        
+        r = gettimeofday(&tv, NULL);
+        if (r) {
+                log_log(LOG_WARNING, "Unable to get time of day: (%d) %s\n",
+                        errno,
+                        strerror(errno));
+                object->error = true;
+                return;
+        }
+    
+        tm_info = localtime(&tv.tv_sec);
+        if (tm_info == NULL) {
+                log_log(LOG_WARNING, "Unable to determine local time\n");
+                object->error = true;
+                return;
+        }
+        
+        strftime(buffer, sizeof(buffer)-1, "%Y-%m-%dT%T", tm_info);
+        strftime(tz, sizeof(tz)-1, "%z", tm_info);
+        snprintf(
+                timestamp,
+                sizeof(timestamp),
+                "%s.%06ld%s",
+                buffer,
+                tv.tv_usec,
+                tz);
+        json_add_string(object, "timestamp", timestamp);
+}
+        
+void json_add_object(struct json_object *object,
+                     const char* name,
+                     struct json_object *value)
+{
+        int rc = 0;
+        json_t *jv = NULL;
+        
+        if (object->error) {
+                return;
+        }
+         
+        if (value != NULL && value->error) {
+                object->error = true;
+                return;
+        }
+   
+        jv = value == NULL ? json_null() : value->root;
+ 
+        if (json_is_array(object->root)) {
+                rc = json_array_append_new(object->root, jv);
+        } else if (json_is_object(object->root)) {
+                rc = json_object_set_new(object->root, name,  jv);
+        } else {
+                log_log(LOG_WARNING, "Invalid JSON object type\n");
+                object->error = true;
+        }
+        if (rc) {
+                log_log(LOG_WARNING, "Unable to add object [%s]\n", name);
+                object->error = true;
+        }
+}
+
+char *json_to_string(const struct json_object *object)
+{
+        char *json = NULL;
+
+        if (json_is_invalid(object)) {
+                log_log(LOG_WARNING, "Invalid JSON object, unable to convert to string\n");
+                return NULL;
+        }
+
+        if (object->root == NULL) {
+        	log_log(LOG_WARNING, "object->root is NULL");
+                return NULL;
+        }
+
+        /*
+         * json_dumps uses malloc, so need to call free(json) to release
+         * the memory
+         */
+        json = json_dumps(object->root, 0);
+        if (json == NULL) {
+                log_log(LOG_WARNING, "Unable to convert JSON object to string\n");
+                return NULL;
+        }
+
+        return json;
+}
