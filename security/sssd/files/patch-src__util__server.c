--- src/util/server.c.orig	2016-04-13 10:48:41.000000000 -0400
+++ src/util/server.c	2018-10-01 05:31:54.711854000 -0400
@@ -308,8 +308,11 @@ static void setup_signals(void)
 #ifndef HAVE_PRCTL
         /* If prctl is not defined on the system, try to handle
          * some common termination signals gracefully */
+    (void) sig_segv_abrt; /* unused */
+/*
     CatchSignal(SIGSEGV, sig_segv_abrt);
     CatchSignal(SIGABRT, sig_segv_abrt);
+ */
 #endif
 
 }
