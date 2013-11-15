--- src/istgt_sock.c.orig	2013-11-15 14:36:14.456928355 -0800
+++ src/istgt_sock.c	2013-11-15 14:44:36.341973194 -0800
@@ -173,7 +173,7 @@
 			continue;
 		}
 		/* bind OK */
-		rc = listen(sock, 2);
+		rc = listen(sock, -1);
 		if (rc != 0) {
 			close(sock);
 			sock = -1;
