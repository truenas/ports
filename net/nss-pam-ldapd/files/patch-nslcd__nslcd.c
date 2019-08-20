--- nslcd/nslcd.c.orig	2019-08-20 09:35:39.397720107 -0700
+++ nslcd/nslcd.c	2019-08-20 09:38:47.812141329 -0700
@@ -382,6 +382,7 @@ static void handleconnection(int sock, M
   switch (action)
   {
     case NSLCD_ACTION_CONFIG_GET:       (void)nslcd_config_get(fp, session); break;
+    case NSLCD_ACTION_STATE_GET:        (void)nslcd_state_get(fp, session); break;
     case NSLCD_ACTION_ALIAS_BYNAME:     (void)nslcd_alias_byname(fp, session); break;
     case NSLCD_ACTION_ALIAS_ALL:        (void)nslcd_alias_all(fp, session); break;
     case NSLCD_ACTION_ETHER_BYNAME:     (void)nslcd_ether_byname(fp, session); break;
@@ -852,13 +853,13 @@ int main(int argc, char *argv[])
   install_sighandler(SIGPIPE, SIG_IGN);
   install_sighandler(SIGTERM, sig_handler);
   install_sighandler(SIGUSR1, sig_handler);
-  install_sighandler(SIGUSR2, SIG_IGN);
+  install_sighandler(SIGUSR2, sig_handler);
   /* signal the starting process to exit because we can provide services now */
   daemonize_ready(EXIT_SUCCESS, NULL);
   /* enable receiving of signals */
   pthread_sigmask(SIG_SETMASK, &oldmask, NULL);
   /* wait until we received a signal */
-  while ((nslcd_receivedsignal == 0) || (nslcd_receivedsignal == SIGUSR1))
+  while ((nslcd_receivedsignal == 0) || (nslcd_receivedsignal == SIGUSR1) || (nslcd_receivedsignal == SIGUSR2))
   {
     sleep(INT_MAX); /* sleep as long as we can or until we receive a signal */
     if (nslcd_receivedsignal == SIGUSR1)
@@ -868,6 +869,13 @@ int main(int argc, char *argv[])
       myldap_immediate_reconnect();
       nslcd_receivedsignal = 0;
     }
+    if (nslcd_receivedsignal == SIGUSR2)
+    {
+      char *ldap_state = myldap_dumpstate();
+      log_log(LOG_WARNING, "%s", ldap_state);
+      free(ldap_state);
+      nslcd_receivedsignal = 0;
+    }
   }
   /* print something about received signal */
   log_log(LOG_INFO, "caught signal %s (%d), shutting down",
