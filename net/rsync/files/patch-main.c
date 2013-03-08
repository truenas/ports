--- /dev/null	2011-10-20 23:09:29.000000000 -0700
+++ ./main.c	2011-10-20 23:11:53.000000000 -0700
@@ -932,6 +932,7 @@
 		rprintf(FERROR,"server_recv: recv_file_list error\n");
 		exit_cleanup(RERR_FILESELECT);
 	}
+
 	if (inc_recurse && file_total == 1)
 		recv_additional_file_list(f_in);
 	verbose = save_verbose;
