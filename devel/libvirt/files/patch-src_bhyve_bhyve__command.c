--- src/bhyve/bhyve_command.c.orig	2021-03-01 10:56:12 UTC
+++ src/bhyve/bhyve_command.c
@@ -213,12 +213,19 @@ bhyveBuildAHCIControllerArgStr(const virDomainDef *def
                            _("unsupported disk device"));
             return -1;
         }
+        if (disk->blockio.logical_block_size) {
+            virBufferAsprintf(&device, ",sectorsize=%d", disk->blockio.logical_block_size);
+            if (disk->blockio.physical_block_size) {
+                virBufferAsprintf(&device, "/%d", disk->blockio.physical_block_size);
+            }
+        }
         virBufferAddBuffer(&buf, &device);
     }
 
     virCommandAddArg(cmd, "-s");
-    virCommandAddArgFormat(cmd, "%d:0,ahci%s",
+    virCommandAddArgFormat(cmd, "%d:%d,ahci%s",
                            controller->info.addr.pci.slot,
+                           controller->info.addr.pci.function,
                            virBufferCurrentContent(&buf));
 
     return 0;
@@ -269,6 +276,7 @@ bhyveBuildVirtIODiskArgStr(const virDomainDef *def G_G
                            virCommandPtr cmd)
 {
     const char *disk_source;
+    virBuffer opt = VIR_BUFFER_INITIALIZER;
 
     if (virDomainDiskTranslateSourcePool(disk) < 0)
         return -1;
@@ -288,10 +296,17 @@ bhyveBuildVirtIODiskArgStr(const virDomainDef *def G_G
 
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
@@ -708,7 +723,6 @@ virBhyveProcessBuildBhyveCmd(bhyveConnPtr driver, virD
      * since it forces the guest to exit when it spins on a lock acquisition.
      */
     virCommandAddArg(cmd, "-H"); /* vmexit from guest on hlt */
-    virCommandAddArg(cmd, "-P"); /* vmexit from guest on pause */
 
     virCommandAddArgList(cmd, "-s", "0:0,hostbridge", NULL);
 
