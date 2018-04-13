--- agent/mibgroup/mibII/tcpTable.c.orig	2017-06-19 15:59:50.000000000 +0800
+++ agent/mibgroup/mibII/tcpTable.c		2017-06-27 20:33:55.000000000 +0800
@@ -170,6 +170,7 @@
      */
     iinfo      = SNMP_MALLOC_TYPEDEF(netsnmp_iterator_info);
     if (!iinfo) {
+		SNMP_FREE(table_info);
         return;
     }
     iinfo->get_first_data_point = tcpTable_first_entry;
