--- shell.c.orig	2011-01-02 21:04:51 UTC
+++ shell.c
@@ -225,6 +225,7 @@
 #else
 int posixly_correct = 0;	/* Non-zero means posix.2 superset. */
 #endif
+int import_functions = 0;	/* Import functions from environment */
 
 /* Some long-winded argument names.  These are obviously new. */
 #define Int 1
@@ -244,6 +245,7 @@
   { "help", Int, &want_initial_help, (char **)0x0 },
   { "init-file", Charp, (int *)0x0, &bashrc_file },
   { "login", Int, &make_login_shell, (char **)0x0 },
+  { "import-functions", Int, &import_functions, (char **)0x0 },
   { "noediting", Int, &no_line_editing, (char **)0x0 },
   { "noprofile", Int, &no_profile, (char **)0x0 },
   { "norc", Int, &no_rc, (char **)0x0 },
