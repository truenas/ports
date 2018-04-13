--- agent/mibgroup/notification/snmpNotifyFilterProfileTable.c.orig	2017-06-19 15:59:52.000000000 +0800
+++ agent/mibgroup/notification/snmpNotifyFilterProfileTable.c		2017-06-27 20:32:39.000000000 +0800
@@ -184,6 +184,7 @@
                               &StorageTmp->snmpTargetParamsName,
                               &StorageTmp->snmpTargetParamsNameLen);
     if (StorageTmp->snmpTargetParamsName == NULL) {
+		SNMP_FREE(StorageTmp);
         config_perror("invalid specification for snmpTargetParamsName");
         return;
     }
