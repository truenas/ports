--- src/bhyve/bhyve_command.c.orig	2020-04-14 22:48:12 UTC
+++ src/bhyve/bhyve_command.c
@@ -220,13 +220,20 @@ bhyveBuildAHCIControllerArgStr(const virDomainDef *def
                            _("unsupported disk device"));
             goto error;
         }
+        if (disk->blockio.logical_block_size) {
+            virBufferAsprintf(&device, ",sectorsize=%d", disk->blockio.logical_block_size);
+            if (disk->blockio.physical_block_size) {
+                virBufferAsprintf(&device, "/%d", disk->blockio.physical_block_size);
+            }
+        }
         virBufferAddBuffer(&buf, &device);
         virBufferFreeAndReset(&device);
     }
 
     virCommandAddArg(cmd, "-s");
-    virCommandAddArgFormat(cmd, "%d:0,ahci%s",
+    virCommandAddArgFormat(cmd, "%d:%d,ahci%s",
                            controller->info.addr.pci.slot,
+                           controller->info.addr.pci.function,
                            virBufferCurrentContent(&buf));
 
     ret = 0;
@@ -280,6 +287,7 @@ bhyveBuildVirtIODiskArgStr(const virDomainDef *def G_G
                            virCommandPtr cmd)
 {
     const char *disk_source;
+    virBuffer opt = VIR_BUFFER_INITIALIZER;
 
     if (virDomainDiskTranslateSourcePool(disk) < 0)
         return -1;
@@ -299,10 +307,17 @@ bhyveBuildVirtIODiskArgStr(const virDomainDef *def G_G
 
     disk_source = virDomainDiskGetSource(disk);
 
+    virBufferAsprintf(&opt, "%d:%d,virtio-blk,%s", disk->info.addr.pci.slot, disk->info.addr.pci.function, disk_source);
+
+    if (disk->blockio.logical_block_size) {
+        virBufferAsprintf(&opt, ",sectorsize=%d", disk->blockio.logical_block_size);
+        if (disk->blockio.physical_block_size) {
+            virBufferAsprintf(&opt, "/%d", disk->blockio.physical_block_size);
+        }
+    }
+
     virCommandAddArg(cmd, "-s");
-    virCommandAddArgFormat(cmd, "%d:0,virtio-blk,%s",
-                           disk->info.addr.pci.slot,
-                           disk_source);
+    virCommandAddArgBuffer(cmd, &opt);
 
     return 0;
 }
@@ -366,7 +381,7 @@ static int
 bhyveBuildLPCArgStr(const virDomainDef *def G_GNUC_UNUSED,
                     virCommandPtr cmd)
 {
-    virCommandAddArgList(cmd, "-s", "1,lpc", NULL);
+    virCommandAddArgList(cmd, "-s", "31,lpc", NULL);
     return 0;
 }
 
@@ -425,17 +440,6 @@ bhyveBuildGraphicsArgStr(const virDomainDef *def,
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
@@ -457,6 +461,9 @@ bhyveBuildGraphicsArgStr(const virDomainDef *def,
         }
 
         virBufferAsprintf(&opt, ":%d", graphics->data.vnc.port);
+        if (graphics->data.vnc.auth.passwd) {
+            virBufferAsprintf(&opt, ",password=%s", graphics->data.vnc.auth.passwd);
+        }
         break;
     case VIR_DOMAIN_GRAPHICS_LISTEN_TYPE_SOCKET:
     case VIR_DOMAIN_GRAPHICS_LISTEN_TYPE_NONE:
@@ -582,7 +589,6 @@ virBhyveProcessBuildBhyveCmd(bhyveConnPtr driver, virD
      * since it forces the guest to exit when it spins on a lock acquisition.
      */
     virCommandAddArg(cmd, "-H"); /* vmexit from guest on hlt */
-    virCommandAddArg(cmd, "-P"); /* vmexit from guest on pause */
 
     virCommandAddArgList(cmd, "-s", "0:0,hostbridge", NULL);
 
