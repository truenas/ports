--- psutil/_psutil_bsd.c.orig
+++ psutil/_psutil_bsd.c
@@ -526,6 +526,56 @@ error:
     return NULL;
 }
 
+
+/*
+ * Return process jail ID
+ */
+PyObject *
+psutil_proc_jid(PyObject *self, PyObject *args) {
+    int i, cnt = -1;
+    long pid;
+    kvm_t *kd;
+    char errbuf[_POSIX2_LINE_MAX];
+    struct kinfo_proc *p;
+
+    if (!PyArg_ParseTuple(args, "l", &pid))
+        return NULL;
+
+    kd = kvm_openfiles(NULL, "/dev/null", NULL, 0, errbuf);
+    if (!kd) {
+        // convert_kvm_err("kvm_openfiles", errbuf);
+        return NULL;
+    }
+
+    p = kvm_getprocs(kd, KERN_PROC_PID, pid, &cnt);
+    if (!p) {
+        NoSuchProcess("kvm_getprocs");
+        kvm_close(kd);
+        return NULL;
+    }
+    if (cnt <= 0) {
+        NoSuchProcess(cnt < 0 ? kvm_geterr(kd) : "kvm_getprocs: cnt==0");
+        kvm_close(kd);
+        return NULL;
+    }
+
+    // On *BSD kernels there are a few kernel-only system processes without an
+    // environment (See e.g. "procstat -e 0 | 1 | 2 ..." on FreeBSD.)
+    // Some system process have no stats attached at all
+    // (they are marked with P_SYSTEM.)
+    // On FreeBSD, it's possible that the process is swapped or paged out,
+    // then there no access to the environ stored in the process' user area.
+    // On NetBSD, we cannot call kvm_getenvv2() for a zombie process.
+    // To make unittest suite happy, return an empty environment.
+    if (!((p)->ki_flag & P_INMEM) || ((p)->ki_flag & P_SYSTEM)) {
+        kvm_close(kd);
+        return Py_BuildValue("i", 0);
+    }
+
+    kvm_close(kd);
+    return Py_BuildValue("i", p->ki_jid);
+}
+
 /*
  * Return the number of logical CPUs in the system.
  * XXX this could be shared with macOS
@@ -1098,6 +1147,8 @@ static PyMethodDef mod_methods[] = {
      "Set process resource limits."},
     {"cpu_count_phys", psutil_cpu_count_phys, METH_VARARGS,
      "Return an XML string to determine the number physical CPUs."},
+    {"proc_jail_id", psutil_proc_jid, METH_VARARGS,
+     "Return process jail ID."},
 #endif
     {"proc_environ", psutil_proc_environ, METH_VARARGS,
      "Return process environment"},
