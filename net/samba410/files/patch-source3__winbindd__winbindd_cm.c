--- source3/winbindd/winbindd_cm.c.orig	2019-01-15 02:07:00.000000000 -0800
+++ source3/winbindd/winbindd_cm.c	2019-07-01 11:03:19.549343068 -0700
@@ -85,10 +85,14 @@
 #include "auth/credentials/credentials.h"
 #include "lib/param/param.h"
 #include "lib/gencache.h"
+#include <stdio.h>
+#include <fcntl.h>
 
 #undef DBGC_CLASS
 #define DBGC_CLASS DBGC_WINBIND
 
+#define WB_ALERT "/var/tmp/.wb_alert"
+
 struct dc_name_ip {
 	fstring name;
 	struct sockaddr_storage ss;
@@ -144,9 +148,38 @@ static void msg_failed_to_go_online(stru
 }
 
 /****************************************************************
- Actually cause a reconnect from a message.
+ Generate and remove sentinel files
 ****************************************************************/
+static void sentinel_go_offline()
+{
+	int fd;
+	DBG_DEBUG("Generating winbind alert sentinel file.\n");
+	fd = open(WB_ALERT, O_CREAT, 0600);
+	if (fd == -1) {
+		DBG_ERR("Failed to generate winbind alert sentinel file: %s.\n",
+			strerror(errno));
+		return;
+	}
+	close(fd);
+} 
 
+static void sentinel_go_online()
+{
+	int ret;
+	if (access(WB_ALERT, F_OK) < 0) {
+		DBG_DEBUG("No winbind alert sentinel file detected.\n");
+		return; 
+	} 
+	DBG_DEBUG("Removing winbind alert sentinel file.\n");
+	ret = unlink(WB_ALERT);
+	if (ret == -1) {
+		DBG_ERR("Failed to remove winbind alert sentinel file: %s.\n",
+			strerror(errno));
+	}
+}
+/****************************************************************
+ Actually cause a reconnect from a message.
+****************************************************************/
 static void msg_try_to_go_online(struct messaging_context *msg,
 				 void *private_data,
 				 uint32_t msg_type,
@@ -472,6 +505,7 @@ void set_domain_offline(struct winbindd_
 					   (const uint8_t *)domain->name,
 					   strlen(domain->name)+1);
 		}			
+		sentinel_go_offline();
 	}
 
 	return;	
@@ -558,6 +592,7 @@ static void set_domain_online(struct win
 					   (const uint8_t *)domain->name,
 					   strlen(domain->name)+1);
 		}			
+		sentinel_go_online();
 	}
 
 	return;	
