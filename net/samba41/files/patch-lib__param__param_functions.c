--- lib/param/param_functions.c	2014-09-08 02:26:14.000000000 -0700
+++ lib/param/param_functions.c	2015-11-23 22:05:43.000000000 -0800
@@ -153,6 +153,7 @@
 FN_LOCAL_PARM_BOOL(kernel_change_notify, bKernelChangeNotify)
 FN_LOCAL_BOOL(durable_handles, bDurableHandles)
 
+FN_GLOBAL_BOOL(ads_dns_update, ads_dns_update)
 FN_GLOBAL_BOOL(allow_insecure_widelinks, bAllowInsecureWidelinks)
 FN_GLOBAL_BOOL(allow_trusted_domains, bAllowTrustedDomains)
 FN_GLOBAL_BOOL(async_smb_echo_handler, bAsyncSMBEchoHandler)
