diff --git a/source3/smbd/oplock.c b/source3/smbd/oplock.c
index 21792bd..7b0a979 100644
--- source3/smbd/oplock.c
+++ source3/smbd/oplock.c
@@ -150,6 +150,8 @@ static void downgrade_file_oplock(files_struct *fsp)
 	sconn->oplocks.exclusive_open--;
 	sconn->oplocks.level_II_open++;
 	fsp->sent_oplock_break = NO_BREAK_SENT;
+
+	TALLOC_FREE(fsp->oplock_timeout);
 }
 
 /****************************************************************************
