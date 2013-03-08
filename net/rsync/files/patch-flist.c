--- /dev/null	2011-10-20 23:09:29.000000000 -0700
+++ ./flist.c	2011-10-20 23:11:41.000000000 -0700
@@ -1455,6 +1455,7 @@
 		if (preserve_acls && !S_ISLNK(file->mode)) {
 			sx.st.st_mode = file->mode;
 			sx.acc_acl = sx.def_acl = NULL;
+			sx.nfs4_acl = NULL;
 			if (get_acl(fname, &sx) < 0) {
 				io_error |= IOERR_GENERAL;
 				return NULL;
