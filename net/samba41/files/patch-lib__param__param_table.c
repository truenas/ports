--- lib/param/param_table.c	2015-08-31 23:45:32.000000000 -0700
+++ lib/param/param_table.c	2015-11-23 22:05:49.000000000 -0800
@@ -4192,6 +4192,15 @@
 
 	{N_("DNS options"), P_SEP, P_SEPARATOR},
 	{
+		.label		= "ads dns update",
+		.type		= P_BOOL,
+		.p_class	= P_GLOBAL,
+		.offset		= GLOBAL_VAR(ads_dns_update),
+		.special	= NULL,
+		.enum_list	= enum_bool_auto,
+		.flags		= FLAG_ADVANCED,
+	},
+	{
 		.label		= "allow dns updates",
 		.type		= P_ENUM,
 		.p_class	= P_GLOBAL,
