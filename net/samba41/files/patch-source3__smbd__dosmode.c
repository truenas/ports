index 2d07dd9..00073fd 100644
--- source3/smbd/dosmode.c 
+++ source3/smbd/dosmode.c 
@@ -25,6 +25,43 @@
 #include "smbd/smbd.h"
 #include "lib/param/loadparm.h"
 
+static NTSTATUS get_file_handle_for_metadata(connection_struct *conn,
+				struct smb_filename *smb_fname,
+				files_struct **ret_fsp,
+				bool *need_close);
+
+static void dos_mode_debug_print(uint32_t mode)
+{
+	DEBUG(8,("dos_mode returning "));
+
+	if (mode & FILE_ATTRIBUTE_HIDDEN) {
+		DEBUG(8, ("h"));
+	}
+	if (mode & FILE_ATTRIBUTE_READONLY) {
+		DEBUG(8, ("r"));
+	}
+	if (mode & FILE_ATTRIBUTE_SYSTEM) {
+		DEBUG(8, ("s"));
+	}
+	if (mode & FILE_ATTRIBUTE_DIRECTORY) {
+		DEBUG(8, ("d"));
+	}
+	if (mode & FILE_ATTRIBUTE_ARCHIVE) {
+		DEBUG(8, ("a"));
+	}
+	if (mode & FILE_ATTRIBUTE_SPARSE) {
+		DEBUG(8, ("[sparse]"));
+	}
+	if (mode & FILE_ATTRIBUTE_OFFLINE) {
+		DEBUG(8, ("[offline]"));
+	}
+	if (mode & FILE_ATTRIBUTE_COMPRESSED) {
+		DEBUG(8, ("[compressed]"));
+	}
+
+	DEBUG(8,("\n"));
+}
+
 static uint32_t filter_mode_by_protocol(uint32_t mode)
 {
 	if (get_Protocol() <= PROTOCOL_LANMAN2) {
@@ -387,6 +424,7 @@ static bool set_ea_dos_attribute(connection_struct *conn,
 			     SAMBA_XATTR_DOS_ATTRIB, blob.data, blob.length,
 			     0) == -1) {
 		bool ret = false;
+		bool need_close = false;
 		files_struct *fsp = NULL;
 
 		if((errno != EPERM) && (errno != EACCES)) {
@@ -418,14 +456,17 @@ static bool set_ea_dos_attribute(connection_struct *conn,
 		}
 
 		/*
-		 * We need to open the file with write access whilst
-		 * still in our current user context. This ensures we
-		 * are not violating security in doing the setxattr.
+		 * We need to get an open file handle to do the
+		 * metadata operation under root.
 		 */
 
-		if (!NT_STATUS_IS_OK(open_file_fchmod(conn, smb_fname,
-						      &fsp)))
+		if (!NT_STATUS_IS_OK(get_file_handle_for_metadata(conn,
+						smb_fname,
+						&fsp,
+						&need_close))) {
 			return false;
+		}
+
 		become_root();
 		if (SMB_VFS_FSETXATTR(fsp,
 				     SAMBA_XATTR_DOS_ATTRIB, blob.data,
@@ -433,7 +474,9 @@ static bool set_ea_dos_attribute(connection_struct *conn,
 			ret = true;
 		}
 		unbecome_root();
-		close_file(NULL, fsp, NORMAL_CLOSE);
+		if (need_close) {
+			close_file(NULL, fsp, NORMAL_CLOSE);
+		}
 		return ret;
 	}
 	DEBUG(10,("set_ea_dos_attribute: set EA 0x%x on file %s\n",
@@ -711,6 +754,8 @@ int file_set_dosmode(connection_struct *conn, struct smb_filename *smb_fname,
 	uint32_t old_mode;
 	struct timespec new_create_timespec;
 	files_struct *fsp = NULL;
+	bool need_close = false;
+	NTSTATUS status;
 
 	if (!CAN_WRITE(conn)) {
 		errno = EROFS;
@@ -879,17 +924,25 @@ int file_set_dosmode(connection_struct *conn, struct smb_filename *smb_fname,
 	}
 
 	/*
-	 * We need to open the file with write access whilst
-	 * still in our current user context. This ensures we
-	 * are not violating security in doing the fchmod.
+	 * We need to get an open file handle to do the
+	 * metadata operation under root.
 	 */
-	if (!NT_STATUS_IS_OK(open_file_fchmod(conn, smb_fname,
-			     &fsp)))
+
+	status = get_file_handle_for_metadata(conn,
+					      smb_fname,
+					      &fsp,
+					      &need_close);
+	if (!NT_STATUS_IS_OK(status)) {
+		errno = map_errno_from_nt_status(status);
 		return -1;
+	}
+
 	become_root();
 	ret = SMB_VFS_FCHMOD(fsp, unixmode);
 	unbecome_root();
-	close_file(NULL, fsp, NORMAL_CLOSE);
+	if (need_close) {
+		close_file(NULL, fsp, NORMAL_CLOSE);
+	}
 	if (!newfile) {
 		notify_fname(conn, NOTIFY_ACTION_MODIFIED,
 			     FILE_NOTIFY_CHANGE_ATTRIBUTES,
@@ -1125,3 +1178,62 @@ struct timespec get_change_timespec(connection_struct *conn,
 {
 	return smb_fname->st.st_ex_mtime;
 }
+
+/****************************************************************************
+ Get a real open file handle we can do meta-data operations on. As it's
+ going to be used under root access only on meta-data we should look for
+ any existing open file handle first, and use that in preference (also to
+ avoid kernel self-oplock breaks). If not use an INTERNAL_OPEN_ONLY handle.
+****************************************************************************/
+
+static NTSTATUS get_file_handle_for_metadata(connection_struct *conn,
+				struct smb_filename *smb_fname,
+				files_struct **ret_fsp,
+				bool *need_close)
+{
+	NTSTATUS status;
+	files_struct *fsp;
+	struct file_id file_id;
+
+	*need_close = false;
+
+	if (!VALID_STAT(smb_fname->st)) {
+		return NT_STATUS_INVALID_PARAMETER;
+	}
+
+	file_id = vfs_file_id_from_sbuf(conn, &smb_fname->st);
+
+	for(fsp = file_find_di_first(conn->sconn, file_id);
+			fsp;
+			fsp = file_find_di_next(fsp)) {
+		if (fsp->fh->fd != -1) {
+			*ret_fsp = fsp;
+			return NT_STATUS_OK;
+		}
+	}
+
+	/* Opens an INTERNAL_OPEN_ONLY write handle. */
+	status = SMB_VFS_CREATE_FILE(
+		conn,                                   /* conn */
+		NULL,                                   /* req */
+		0,                                      /* root_dir_fid */
+		smb_fname,                              /* fname */
+		FILE_WRITE_DATA,                        /* access_mask */
+		(FILE_SHARE_READ | FILE_SHARE_WRITE |   /* share_access */
+			FILE_SHARE_DELETE),
+		FILE_OPEN,                              /* create_disposition*/
+		0,                                      /* create_options */
+		0,                                      /* file_attributes */
+		INTERNAL_OPEN_ONLY,                     /* oplock_request */
+                0,                                      /* allocation_size */
+		0,                                      /* private_flags */
+		NULL,                                   /* sd */
+		NULL,                                   /* ea_list */
+		ret_fsp,                                /* result */
+		NULL);                                  /* pinfo */
+
+	if (NT_STATUS_IS_OK(status)) {
+		*need_close = true;
+	}
+	return status;
+}
