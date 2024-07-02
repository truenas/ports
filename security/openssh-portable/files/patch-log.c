--- log.c.orig	2021-09-26 09:03:19.000000000 -0500
+++ log.c	2024-07-02 08:11:23.318041000 -0500
@@ -451,12 +451,14 @@ void
 sshsigdie(const char *file, const char *func, int line, int showfunc,
     LogLevel level, const char *suffix, const char *fmt, ...)
 {
+#if 0
 	va_list args;
 
 	va_start(args, fmt);
 	sshlogv(file, func, line, showfunc, SYSLOG_LEVEL_FATAL,
 	    suffix, fmt, args);
 	va_end(args);
+#endif
 	_exit(1);
 }
 
