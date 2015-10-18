--- variables.c.orig	2014-09-26 19:56:56 UTC
+++ variables.c
@@ -100,6 +100,7 @@
 extern int assigning_in_environment;
 extern int executing_builtin;
 extern int funcnest_max;
+extern int import_functions;
 
 #if defined (READLINE)
 extern int no_line_editing;
@@ -338,7 +339,7 @@
 
       /* If exported function, define it now.  Don't import functions from
 	 the environment in privileged mode. */
-      if (privmode == 0 && read_but_dont_execute == 0 && STREQN ("() {", string, 4))
+      if (import_functions && privmode == 0 && read_but_dont_execute == 0 && STREQN ("() {", string, 4))
 	{
 	  string_length = strlen (string);
 	  temp_string = (char *)xmalloc (3 + string_length + char_index);
