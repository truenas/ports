--- mDNSShared/mDNSDebug.c.orig	2014-06-23 00:47:14.000000000 -0700
+++ mDNSShared/mDNSDebug.c	2014-06-23 00:48:12.000000000 -0700
@@ -65,9 +65,11 @@
 // Log message with default "mDNSResponder" ident string at the start
 mDNSlocal void LogMsgWithLevelv(mDNSLogLevel_t logLevel, const char *format, va_list ptr)
 {
+#if 0	/* mDNSResponder too chatty for FreeNAS - make it STFU by default */
     char buffer[512];
     buffer[mDNS_vsnprintf((char *)buffer, sizeof(buffer), format, ptr)] = 0;
     mDNSPlatformWriteLogMsg(ProgramName, buffer, logLevel);
+#endif
 }
 
 #define LOG_HELPER_BODY(L) \
