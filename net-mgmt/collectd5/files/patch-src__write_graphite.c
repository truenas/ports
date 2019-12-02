--- src/write_graphite.c.orig
+++ src/write_graphite.c
@@ -199,6 +199,71 @@ static int wg_flush_nolock(cdtime_t timeout, struct wg_callback *cb) {
   return status;
 }
 
+static int connect_wait(int sockno, struct sockaddr *addr, size_t addrlen, struct timeval *timeout)
+{
+  int res, opt;
+
+  // get socket flags
+  if ((opt = fcntl(sockno, F_GETFL, NULL)) < 0) {
+    return -1;
+  }
+
+  // set socket non-blocking
+  if (fcntl(sockno, F_SETFL, opt | O_NONBLOCK) < 0) {
+    return -1;
+  }
+
+  // try to connect
+  if ((res = connect(sockno, addr, addrlen)) < 0) {
+    if (errno == EINPROGRESS) {
+      fd_set wait_set;
+
+      // make file descriptor set with socket
+      FD_ZERO(&wait_set);
+      FD_SET(sockno, &wait_set);
+
+      // wait for socket to be writable; return after given timeout
+      res = select(sockno + 1, NULL, &wait_set, NULL, timeout);
+    }
+  }
+  // connection was successful immediately
+  else {
+    res = 1;
+  }
+
+  // reset socket flags
+  if (fcntl(sockno, F_SETFL, opt) < 0) {
+    return -1;
+  }
+
+  // an error occured in connect or select
+  if (res < 0) {
+    return -1;
+  }
+  // select timed out
+  else if (res == 0) {
+    errno = ETIMEDOUT;
+    return 1;
+  }
+  // almost finished...
+  else {
+    socklen_t len = sizeof (opt);
+
+    // check for errors in socket layer
+    if (getsockopt(sockno, SOL_SOCKET, SO_ERROR, &opt, &len) < 0) {
+      return -1;
+    }
+
+    // there was an error
+    if (opt) {
+      errno = opt;
+      return -1;
+    }
+  }
+
+  return 0;
+}
+
 static int wg_callback_init(struct wg_callback *cb) {
   struct addrinfo *ai_list;
   cdtime_t now;
@@ -243,7 +308,10 @@ static int wg_callback_init(struct wg_callback *cb) {
 
     set_sock_opts(cb->sock_fd);
 
-    status = connect(cb->sock_fd, ai_ptr->ai_addr, ai_ptr->ai_addrlen);
+    struct timeval timeout;
+    timeout.tv_sec = 10;
+    timeout.tv_usec = 0;
+    status = connect_wait(cb->sock_fd, ai_ptr->ai_addr, ai_ptr->ai_addrlen, &timeout);
     if (status != 0) {
       snprintf(connerr, sizeof(connerr), "failed to connect to remote host: %s",
                STRERRNO);
