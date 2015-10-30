--- setup.py.orig	2015-05-04 16:14:22 UTC
+++ setup.py
@@ -127,8 +127,8 @@ VERSION = open('VERSION').read().strip()
 debug = '--debug' in sys.argv
 
 # Global variables
-lib_dirs = []
-inc_dirs = ['c-blosc/hdf5']
+lib_dirs = ['%%LOCALBASE%%/lib']
+inc_dirs = ['c-blosc/hdf5','%%LOCALBASE%%/include']
 optional_libs = []
 data_files = []    # list of data files to add to packages (mainly for DLL's)
 
@@ -761,7 +761,7 @@ if 'BLOSC' not in optional_libs:
         finally:
             os.remove(fd.name)
 
-    try_flags = ["-march=native", "-msse2"]
+    try_flags = ["-msse2"]
     for ff in try_flags:
         if compiler_has_flags(compiler, [ff]):
             print("Setting compiler flag: " + ff)
