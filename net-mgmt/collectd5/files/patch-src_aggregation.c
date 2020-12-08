--- src/aggregation.c.orig
+++ src/aggregation.c
@@ -380,6 +389,9 @@ static int agg_instance_read(agg_instance_t *inst, cdtime_t t) /* {{{ */
   sstrncpy(vl.host, inst->ident.host, sizeof(vl.host));
   sstrncpy(vl.plugin, inst->ident.plugin, sizeof(vl.plugin));
   sstrncpy(vl.type, inst->ident.type, sizeof(vl.type));
+  if (strcmp(vl.type, "percent") == 0) {
+    sstrncpy(vl.type, "gauge", sizeof(vl.type));
+  }
   sstrncpy(vl.type_instance, inst->ident.type_instance,
            sizeof(vl.type_instance));
 
