--- avahi-daemon/static-services.c.orig	2019-08-22 08:18:03.904626927 -0700
+++ avahi-daemon/static-services.c	2019-08-22 08:18:10.592838416 -0700
@@ -59,6 +59,7 @@ struct StaticService {
     char *domain_name;
     char *host_name;
     uint16_t port;
+    int interface;
     int protocol;
 
     AvahiStringList *subtypes;
@@ -123,6 +124,7 @@ static StaticService *static_service_new(StaticService
     s->type = s->host_name = s->domain_name = NULL;
     s->port = 0;
     s->protocol = AVAHI_PROTO_UNSPEC;
+    s->interface = AVAHI_IF_UNSPEC;
 
     s->txt_records = NULL;
     s->subtypes = NULL;
@@ -256,7 +258,7 @@ static void add_static_service_group_to_server(StaticS
         if (avahi_server_add_service_strlst(
                 avahi_server,
                 g->entry_group,
-                AVAHI_IF_UNSPEC, s->protocol,
+                s->interface, s->protocol,
                 0,
                 g->chosen_name, s->type, s->domain_name,
                 s->host_name, s->port,
@@ -273,7 +275,7 @@ static void add_static_service_group_to_server(StaticS
             if (avahi_server_add_service_subtype(
                     avahi_server,
                     g->entry_group,
-                    AVAHI_IF_UNSPEC, s->protocol,
+                    s->interface, s->protocol,
                     0,
                     g->chosen_name, s->type, s->domain_name,
                     (char*) i->text) < 0) {
@@ -305,6 +307,7 @@ typedef enum {
     XML_TAG_DOMAIN_NAME,
     XML_TAG_HOST_NAME,
     XML_TAG_PORT,
+    XML_TAG_INTERFACE,
     XML_TAG_TXT_RECORD
 } xml_tag_name;
 
@@ -410,6 +413,11 @@ static void XMLCALL xml_start(void *data, const char *
             goto invalid_attr;
 
         u->current_tag = XML_TAG_PORT;
+    } else if (u->current_tag == XML_TAG_SERVICE && strcmp(el, "interface") == 0) {
+        if (attr[0])
+            goto invalid_attr;
+
+        u->current_tag = XML_TAG_INTERFACE;
     } else if (u->current_tag == XML_TAG_SERVICE && strcmp(el, "txt-record") == 0) {
         if (attr[0]) {
             if (strcmp(attr[0], "value-format") == 0) {
@@ -617,6 +625,15 @@ static void XMLCALL xml_end(void *data, AVAHI_GCC_UNUS
             break;
         }
 
+        case XML_TAG_INTERFACE: {
+            int i;
+            assert(u->service);
+            i = u->buf ? atoi(u->buf) : 0;
+            u->service->interface = i;
+            u->current_tag = XML_TAG_SERVICE;
+            break;
+        }
+
         case XML_TAG_TXT_RECORD: {
             assert(u->service);
             if (u->txt_key != NULL) {
@@ -731,6 +748,7 @@ static void XMLCALL xml_cdata(void *data, const XML_Ch
             break;
 
         case XML_TAG_PORT:
+        case XML_TAG_INTERFACE:
         case XML_TAG_SUBTYPE:
             assert(u->service);
             u->buf = append_cdata(u->buf, s, len);
