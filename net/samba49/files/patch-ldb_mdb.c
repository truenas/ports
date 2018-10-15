--- lib/ldb/ldb_mdb/ldb_mdb.c.orig	2018-10-10 06:54:45.149371000 -0400
+++ lib/ldb/ldb_mdb/ldb_mdb.c	2018-10-10 06:55:06.916075000 -0400
@@ -40,7 +40,6 @@ int ldb_mdb_err_map(int lmdb_err)
 		return LDB_SUCCESS;
 	case EIO:
 		return LDB_ERR_OPERATIONS_ERROR;
-	case EBADE:
 	case MDB_INCOMPATIBLE:
 	case MDB_CORRUPTED:
 	case MDB_INVALID:
