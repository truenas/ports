--- src/bhyve/bhyve_command.c.orig	2019-08-05 00:05:44 UTC
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
