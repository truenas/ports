--- ldb_mdb/ldb_mdb.c.orig	2018-10-11 06:35:33.801188000 -0400
+++ ldb_mdb/ldb_mdb.c	2018-10-11 06:35:50.903862000 -0400
@@ -40,7 +40,6 @@ int ldb_mdb_err_map(int lmdb_err)
 		return LDB_SUCCESS;
 	case EIO:
 		return LDB_ERR_OPERATIONS_ERROR;
-	case EBADE:
 	case MDB_INCOMPATIBLE:
 	case MDB_CORRUPTED:
 	case MDB_INVALID:
