--- agent/mibgroup/host/data_access/swrun_kinfo.c.orig	2014-12-08 23:23:22.000000000 +0300
+++ agent/mibgroup/host/data_access/swrun_kinfo.c	2018-04-13 12:31:35.575373074 +0300
@@ -256,8 +256,6 @@
             strlcat(buf, *argv, sizeof(buf));
             argv++;
         }
-        if (strlen(buf) >= BUFSIZ-10)
-            snmp_log(LOG_ERR, "params %lu/%d %s\n", strlen(buf), BUFSIZ, buf);
         entry->hrSWRunParameters_len =
             sprintf(entry->hrSWRunParameters, "%.*s",
                     (int)sizeof(entry->hrSWRunParameters) - 1, buf+1);
