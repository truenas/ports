--- agent/mibgroup/agent/nsLogging.c.orig	2017-06-19 15:59:52.000000000 +0800
+++ agent/mibgroup/agent/nsLogging.c		2017-06-27 20:31:48.000000000 +0800
@@ -54,6 +54,7 @@
      */
     iinfo      = SNMP_MALLOC_TYPEDEF(netsnmp_iterator_info);
     if (!iinfo) {
+		SNMP_FREE(table_info);
         return;
     }
     iinfo->get_first_data_point = get_first_logging_entry;
