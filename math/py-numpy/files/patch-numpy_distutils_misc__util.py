--- numpy/distutils/misc_util.py.orig	2023-05-03 12:50:14 UTC
+++ numpy/distutils/misc_util.py
@@ -89,10 +89,7 @@ def get_num_build_jobs():
 
     """
     from numpy.distutils.core import get_distribution
-    try:
-        cpu_count = len(os.sched_getaffinity(0))
-    except AttributeError:
-        cpu_count = multiprocessing.cpu_count()
+    cpu_count = multiprocessing.cpu_count()
     cpu_count = min(cpu_count, 8)
     envjobs = int(os.environ.get("NPY_NUM_BUILD_JOBS", cpu_count))
     dist = get_distribution()
