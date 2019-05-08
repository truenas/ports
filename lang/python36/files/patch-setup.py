--- setup.py.orig	2018-12-23 21:37:14 UTC
+++ setup.py
@@ -48,7 +48,10 @@ host_platform = get_platform()
 COMPILED_WITH_PYDEBUG = ('--with-pydebug' in sysconfig.get_config_var("CONFIG_ARGS"))
 
 # This global variable is used to hold the list of modules to be disabled.
-disabled_module_list = []
+try:
+    disabled_module_list = sysconfig.get_config_var("DISABLED_EXTENSIONS").split(" ")
+except KeyError:
+    disabled_module_list = list()
 
 def add_dir_to_list(dirlist, dir):
     """Add the directory 'dir' to the list 'dirlist' (after any relative
