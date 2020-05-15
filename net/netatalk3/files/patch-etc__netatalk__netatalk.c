--- etc/netatalk/netatalk.c.orig	2018-12-19 23:10:29.000000000 -0800
+++ etc/netatalk/netatalk.c	2020-05-15 09:44:00.200973152 -0700
@@ -54,6 +54,9 @@
 #define NETATALK_SRV_OPTIONAL 0
 #define NETATALK_SRV_ERROR    NETATALK_SRV_NEEDED
 
+#define NETATALK_PIDFILE "/var/run/netatalk.pid"
+#define PIDDIR "/var/run"
+
 /* forward declaration */
 static pid_t run_process(const char *path, ...);
 static void kill_childs(int sig, ...);
@@ -300,6 +303,8 @@ static void kill_childs(int sig, ...)
 static void netatalk_exit(int ret)
 {
     server_unlock(PATH_NETATALK_LOCK);
+    pidfile_unlink(PIDDIR, "afpd");
+    pidfile_unlink(PIDDIR, "netatalk");
     exit(ret);
 }
 
@@ -444,6 +449,11 @@ int main(int argc, char **argv)
     event_set_fatal_callback(netatalk_exit);
 
     LOG(log_note, logtype_default, "Netatalk AFP server starting");
+
+    if (access(NETATALK_PIDFILE, F_OK) == 0) {
+        pidfile_unlink(PIDDIR, "netatalk");
+    }
+    pidfile_create(PIDDIR, "netatalk");
 
     if ((afpd_pid = run_process(_PATH_AFPD, "-d", "-F", obj.options.configfile, NULL)) == NETATALK_SRV_ERROR) {
         LOG(log_error, logtype_afpd, "Error starting 'afpd'");
