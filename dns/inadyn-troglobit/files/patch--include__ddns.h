--- include/ddns.h.orig
+++ include/ddns.h
@@ -50,7 +50,7 @@
 #define DDNS_FORCED_UPDATE_PERIOD         (30 * 24 * 3600)        /* 30 days in sec */
 #define DDNS_DEFAULT_CMD_CHECK_PERIOD     1       /* sec */
 #define DDNS_DEFAULT_ITERATIONS           0       /* Forever */
-#define DDNS_HTTP_RESPONSE_BUFFER_SIZE    BUFSIZ  /* 8 kiB */
+#define DDNS_HTTP_RESPONSE_BUFFER_SIZE    8192    /* 8 kiB */
 #define DDNS_HTTP_REQUEST_BUFFER_SIZE     2500    /* Bytes */
 #define DDNS_MAX_ALIAS_NUMBER             10      /* maximum number of aliases per server that can be maintained */
 #define DDNS_MAX_SERVER_NUMBER            5       /* maximum number of servers that can be maintained */
