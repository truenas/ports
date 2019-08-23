--- src/bhyve/bhyve_command.c.orig	2019-04-27 10:31:34 UTC
+++ src/bhyve/bhyve_command.c
@@ -236,8 +236,9 @@ bhyveBuildAHCIControllerArgStr(const virDomainDef *def
         goto error;

     virCommandAddArg(cmd, "-s");
-    virCommandAddArgFormat(cmd, "%d:0,ahci%s",
+    virCommandAddArgFormat(cmd, "%d:%d,ahci%s",
                            controller->info.addr.pci.slot,
+                           controller->info.addr.pci.function,
                            virBufferCurrentContent(&buf));

     ret = 0;
@@ -311,8 +312,9 @@ bhyveBuildVirtIODiskArgStr(const virDomainDef *def ATT
     disk_source = virDomainDiskGetSource(disk);

     virCommandAddArg(cmd, "-s");
-    virCommandAddArgFormat(cmd, "%d:0,virtio-blk,%s",
+    virCommandAddArgFormat(cmd, "%d:%d,virtio-blk,%s",
                            disk->info.addr.pci.slot,
+                           disk->info.addr.pci.function,
                            disk_source);

     return 0;
@@ -383,17 +385,6 @@ bhyveBuildGraphicsArgStr(const virDomainDef *def,
             goto error;
         }

-        if (graphics->data.vnc.auth.passwd) {
-            virReportError(VIR_ERR_CONFIG_UNSUPPORTED, "%s",
-                           _("vnc password auth not supported"));
-            goto error;
-        } else {
-             /* Bhyve doesn't support VNC Auth yet, so print a warning about
-              * unauthenticated VNC sessions */
-             VIR_WARN("%s", _("Security warning: currently VNC auth is not"
-                              " supported."));
-        }
-
         if (glisten->address) {
             escapeAddr = strchr(glisten->address, ':') != NULL;
             if (escapeAddr)
@@ -415,6 +406,9 @@ bhyveBuildGraphicsArgStr(const virDomainDef *def,
         }

         virBufferAsprintf(&opt, ":%d", graphics->data.vnc.port);
+        if (graphics->data.vnc.auth.passwd) {
+            virBufferAsprintf(&opt, ",password=%s", graphics->data.vnc.auth.passwd);
+        }
         break;
     case VIR_DOMAIN_GRAPHICS_LISTEN_TYPE_SOCKET:
     case VIR_DOMAIN_GRAPHICS_LISTEN_TYPE_NONE:
