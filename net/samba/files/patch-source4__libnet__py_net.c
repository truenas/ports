--- source4/libnet/py_net.c.orig	2020-11-02 20:59:57.661253000 -0500
+++ source4/libnet/py_net.c	2020-11-02 21:00:34.634111000 -0500
@@ -843,7 +843,7 @@ static PyMethodDef net_obj_methods[] = {
 
 static void py_net_dealloc(py_net_Object *self)
 {
-	talloc_free(self->mem_ctx);
+	talloc_free(self->ev);
 	PyObject_Del(self);
 }
 
