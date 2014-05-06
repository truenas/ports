index 6e4f690..5f7bff9 100644
--- source3/smbd/open.c 
+++ source3/smbd/open.c 
@@ -1079,6 +1079,11 @@ static void validate_my_share_entries(struct smbd_server_connection *sconn,
 		return;
 	}
 
+	if (share_entry->share_file_id == 0) {
+		/* INTERNAL_OPEN_ONLY */
+		return;
+	}
+
 	if (!is_valid_share_mode_entry(share_entry)) {
 		return;
 	}
@@ -2088,9 +2093,12 @@ static NTSTATUS open_file_ntcreate(connection_struct *conn,
 		   create_options, (unsigned int)unx_mode, oplock_request,
 		   (unsigned int)private_flags));
 
-	if ((req == NULL) && ((oplock_request & INTERNAL_OPEN_ONLY) == 0)) {
-		DEBUG(0, ("No smb request but not an internal only open!\n"));
-		return NT_STATUS_INTERNAL_ERROR;
+	if (req == NULL) {
+		/* Ensure req == NULL means INTERNAL_OPEN_ONLY */
+		SMB_ASSERT(((oplock_request & INTERNAL_OPEN_ONLY) != 0));
+	} else {
+		/* And req != NULL means no INTERNAL_OPEN_ONLY */
+		SMB_ASSERT(((oplock_request & INTERNAL_OPEN_ONLY) == 0));
 	}
 
 	/*
@@ -2830,39 +2838,6 @@ static NTSTATUS open_file_ntcreate(connection_struct *conn,
 	return NT_STATUS_OK;
 }
 
-
-/****************************************************************************
- Open a file for for write to ensure that we can fchmod it.
-****************************************************************************/
-
-NTSTATUS open_file_fchmod(connection_struct *conn,
-			  struct smb_filename *smb_fname,
-			  files_struct **result)
-{
-	if (!VALID_STAT(smb_fname->st)) {
-		return NT_STATUS_INVALID_PARAMETER;
-	}
-
-        return SMB_VFS_CREATE_FILE(
-		conn,					/* conn */
-		NULL,					/* req */
-		0,					/* root_dir_fid */
-		smb_fname,				/* fname */
-		FILE_WRITE_DATA,			/* access_mask */
-		(FILE_SHARE_READ | FILE_SHARE_WRITE |	/* share_access */
-		    FILE_SHARE_DELETE),
-		FILE_OPEN,				/* create_disposition*/
-		0,					/* create_options */
-		0,					/* file_attributes */
-		INTERNAL_OPEN_ONLY,			/* oplock_request */
-		0,					/* allocation_size */
-		0,					/* private_flags */
-		NULL,					/* sd */
-		NULL,					/* ea_list */
-		result,					/* result */
-		NULL);					/* pinfo */
-}
-
 static NTSTATUS mkdir_internal(connection_struct *conn,
 			       struct smb_filename *smb_dname,
 			       uint32 file_attributes)
