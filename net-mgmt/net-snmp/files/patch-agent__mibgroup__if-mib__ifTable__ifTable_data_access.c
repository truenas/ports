--- agent/mibgroup/if-mib/ifTable/ifTable_data_access.c.orig	2014-09-24 23:56:32.386420291 +0200
+++ agent/mibgroup/if-mib/ifTable/ifTable_data_access.c	2014-09-25 17:18:38.567333784 +0200
@@ -503,8 +503,13 @@
         /*
          * fix this when we hit an arch that reports its own last change
          */
+#if 0
+	/* 
+	 * That assert is wrong according to net-snmp devs.
+	 */
         netsnmp_assert(0 == (ifentry->ns_flags &
                              NETSNMP_INTERFACE_FLAGS_HAS_LASTCHANGE));
+#endif
         if (0 == _first_load) {
             rowreq_ctx->data.ifLastChange = netsnmp_get_agent_uptime();
             ifTable_lastChange_set(rowreq_ctx->data.ifLastChange);
