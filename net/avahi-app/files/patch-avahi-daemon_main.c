--- avahi-daemon/main.c.orig	2017-07-10 03:14:59.111370487 -0700
+++ avahi-daemon/main.c	2020-06-01 06:57:38.320051863 -0700
@@ -324,6 +324,11 @@ static void update_browse_domains(void) {
         return;
     }
 
+    if (!resolv_conf_search_domains) {
+	    avahi_server_set_browse_domains(avahi_server, NULL);
+	    return;
+    }
+
     l = avahi_string_list_copy(config.server_config.browse_domains);
 
     for (p = resolv_conf_search_domains, n = 0; *p && n < BROWSE_DOMAINS_MAX; p++, n++) {
@@ -1139,9 +1144,6 @@ static int run_server(DaemonConfig *c) {
     assert(c);
 
     ignore_signal(SIGPIPE);
-
-    if (!(nss_support = avahi_nss_support()))
-        avahi_log_warn("WARNING: No NSS support for mDNS detected, consider installing nss-mdns!");
 
     if (!(simple_poll_api = avahi_simple_poll_new())) {
         avahi_log_error("Failed to create main loop object.");
