--- src/data.c.orig
+++ src/data.c
@@ -1135,7 +1135,8 @@ int pr_data_xfer(char *cl_buf, size_t cl_size) {
         while (len < 0) {
           int xerrno = errno;
  
-          if (xerrno == EAGAIN) {
+          if (xerrno == EAGAIN ||
+              xerrno == EINTR) {
             /* Since our socket is in non-blocking mode, read(2) can return
              * EAGAIN if there is no data yet for us.  Handle this by
              * delaying temporarily, then trying again.
@@ -1257,7 +1258,8 @@ int pr_data_xfer(char *cl_buf, size_t cl_size) {
       while (len < 0) {
         int xerrno = errno;
 
-        if (xerrno == EAGAIN) {
+        if (xerrno == EAGAIN ||
+            xerrno == EINTR) {
           /* Since our socket is in non-blocking mode, read(2) can return
            * EAGAIN if there is no data yet for us.  Handle this by
            * delaying temporarily, then trying again.
@@ -1354,7 +1356,8 @@ int pr_data_xfer(char *cl_buf, size_t cl_size) {
       while (bwrote < 0) {
         int xerrno = errno;
 
-        if (xerrno == EAGAIN) {
+        if (xerrno == EAGAIN ||
+            xerrno == EINTR) {
           /* Since our socket is in non-blocking mode, write(2) can return
            * EAGAIN if there is not enough from for our data yet.  Handle
            * this by delaying temporarily, then trying again.
