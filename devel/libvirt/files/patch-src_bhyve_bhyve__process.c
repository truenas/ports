--- src/bhyve/bhyve_process.c.orig	2020-04-24 15:37:02 UTC
+++ src/bhyve/bhyve_process.c
@@ -298,6 +298,8 @@ virBhyveProcessStop(bhyveConnPtr driver,
     if (virCommandRun(cmd, NULL) < 0)
         goto cleanup;
 
+    ignore_value(virProcessKill(vm->pid, SIGTERM));
+
     if ((priv != NULL) && (priv->mon != NULL))
          bhyveMonitorClose(priv->mon);
 
