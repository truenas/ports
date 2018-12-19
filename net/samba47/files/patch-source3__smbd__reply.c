--- source3/smbd/reply.c.orig	2018-12-19 11:07:54 UTC
+++ source3/smbd/reply.c
@@ -6645,9 +6645,17 @@ NTSTATUS rename_internals_fsp(connection
 		return status;
 	}
 
+/*
+	FIXME: OS-X clients will set a RH lease on AFP_Resource streams which causes
+	the following check to prevent users from rename the file until the OS-X client
+	with the lease disconnects.
+	This is incorrect behavior per MS docs.
+ */
+/*
 	if (file_has_open_streams(fsp)) {
 		return NT_STATUS_ACCESS_DENIED;
 	}
+ */
 
 	/* Make a copy of the dst smb_fname structs */
 
