--- source3/utils/net_ads.c	2013-12-05 01:16:48.000000000 -0800
+++ source3/utils/net_ads.c	2015-11-23 22:05:51.000000000 -0800
@@ -1351,6 +1351,10 @@
 		return;
 	}
 
+	if (!lp_ads_dns_update()) {
+		return;
+	}
+
 	if (!r->out.domain_is_ad) {
 		return;
 	}
