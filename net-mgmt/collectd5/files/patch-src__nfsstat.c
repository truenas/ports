--- src/nfsstat.c.orig	2016-05-26 22:25:31 UTC
+++ src/nfsstat.c
@@ -0,0 +1,268 @@
+/**
+ * collectd - src/nfs_freebsd.c 
+ * 
+ * Copyright (c) 2016 iXsystems, Inc.
+ * All rights reserved.
+ *
+ * Redistribution and use in source and binary forms, with or without
+ * modification, are permitted provided that the following conditions
+ * are met:
+ * 1. Redistributions of source code must retain the above copyright
+ *    notice, this list of conditions and the following disclaimer.
+ * 2. Redistributions in binary form must reproduce the above copyright
+ *    notice, this list of conditions and the following disclaimer in the
+ *    documentation and/or other materials provided with the distribution.
+ *
+ * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
+ * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
+ * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
+ * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
+ * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
+ * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
+ * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
+ * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
+ * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
+ * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
+ * SUCH DAMAGE.
+ */
+
+#include "collectd.h"
+#include "utils/common/common.h"
+#include "plugin.h"
+
+#include <sys/param.h>
+#include <sys/module.h>
+#include <sys/mount.h>
+#include <nfs/nfsproto.h>
+#include <nfsclient/nfs.h>
+#include <nfsserver/nfs.h>
+#include <nfs/nfssvc.h>
+#include <fs/nfs/nfsport.h>
+
+#define	NFSSTAT_PLUGIN_NAME		"nfsstat"
+#define	NFSSTAT_CLIENT_ONLY		0x00000001
+#define	NFSSTAT_SERVER_ONLY		0x00000002
+#define	NFSSTAT_PERCENTAGE		0x00000004
+#define	NFSSTAT_ALL				(NFSSTAT_CLIENT_ONLY|NFSSTAT_SERVER_ONLY)
+
+static int nfsstat_flags = NFSSTAT_ALL;
+
+static const char *nfsstat_config_keys[] = {
+	"ClientStats",
+	"ServerStats",
+	"ValuesAbsolute",
+	"ValuesPercentage"
+};
+
+static int nfsstat_config_keys_size = STATIC_ARRAY_SIZE(nfsstat_config_keys);
+
+static int
+nfsstat_init(void)
+{
+	nfsstat_flags = NFSSTAT_ALL;
+	return (0);
+}
+
+static int
+nfsstat_config(const char *key, const char *value)
+{
+	if (strcasecmp(key, "ServerStats") == 0) {
+		if (IS_TRUE(value))
+			nfsstat_flags |= NFSSTAT_SERVER_ONLY;
+
+	} else if (strcasecmp(key, "ClientStats") == 0) {
+		if (IS_TRUE(value))
+			nfsstat_flags |= NFSSTAT_CLIENT_ONLY;
+
+	} else if (strcasecmp(key, "ValuesAbsolute") == 0) {
+		nfsstat_flags &= ~NFSSTAT_PERCENTAGE;
+
+	} else if (strcasecmp(key, "ValuesPercentage") == 0) {
+		nfsstat_flags |= NFSSTAT_PERCENTAGE;
+	}
+
+	return (0);
+}
+
+static int
+nfsstat_server_submit(struct ext_nfsstats *ext_nfsstats, struct nfsstatsv1 *ext_nfsstatsv1)
+{
+	value_t v[1];
+	value_list_t vl = VALUE_LIST_INIT;
+
+	vl.values = v;
+	vl.values_len = STATIC_ARRAY_SIZE(v);
+	sstrncpy(vl.host, hostname_g, sizeof(vl.host));
+	sstrncpy(vl.plugin, NFSSTAT_PLUGIN_NAME, sizeof(vl.plugin));
+	sstrncpy(vl.plugin_instance, "server",
+		sizeof(vl.plugin_instance));
+	sstrncpy(vl.type, NFSSTAT_PLUGIN_NAME, sizeof(vl.type));
+
+#if 0
+	//sstrncpy(vl.type, NFSSTAT_PLUGIN_NAME, sizeof(vl.type));
+	sstrncpy(vl.type, "server", sizeof(vl.type));
+	sstrncpy(vl.type_instance, "server", sizeof(vl.type_instance));
+#endif
+
+	plugin_dispatch_multivalue(&vl,
+		nfsstat_flags & NFSSTAT_PERCENTAGE, DS_TYPE_DERIVE,
+		"access", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_ACCESS],
+		"commit", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_COMMIT],
+		"create", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_CREATE],
+		"fsinfo", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_FSINFO],
+		"fsstat", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_FSSTAT],
+		"getattr", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_GETATTR],
+		"link", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_LINK],
+		"lookup", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_LOOKUP],
+		"mkdir", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_MKDIR],
+		"mknod", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_MKNOD],
+		"pathconf", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_PATHCONF],
+		"read", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_READ],
+		"read_bytes", (derive_t) ext_nfsstatsv1->srvbytes[NFSV4OP_READ],
+		"readdir", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_READDIR],
+		"readirplus", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_READDIRPLUS],
+		"readlink", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_READLINK],
+		"remove", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_REMOVE],
+		"rename", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_RENAME],
+		"rmdir", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_RMDIR],
+		"setattr", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_SETATTR],
+		"symlink", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_SYMLINK],
+		"write", (derive_t) ext_nfsstats->srvrpccnt[NFSV4OP_WRITE],
+		"write_bytes", (derive_t) ext_nfsstatsv1->srvbytes[NFSV4OP_WRITE],
+		NULL
+	);
+
+#if 0
+	"Server Ret-Failed", ext_nfsstats->srvrpc_errs
+	"Server Faults", ext_nfsstats->srv_errs
+
+	"Inprog", ext_nfsstats->srvcache_inproghits
+	"Idem", ext_nfsstats->srvcache_idemdonehits
+	"Non-idem", ext_nfsstats->srvcache_nonidemdonehits
+	"Misses", ext_nfsstats->srvcache_misses
+
+	"WriteOps", ext_nfsstats->srvrpccnt[NFSV4OP_WRITE]
+	"WriteRPC", ext_nfsstats->srvrpccnt[NFSV4OP_WRITE]
+#endif
+
+
+	return (0);
+}
+
+static int
+nfsstat_client_submit(struct ext_nfsstats *ext_nfsstats)
+{
+	value_t v[1];
+	value_list_t vl = VALUE_LIST_INIT;
+
+	vl.values = v;
+	vl.values_len = STATIC_ARRAY_SIZE(v);
+	sstrncpy(vl.host, hostname_g, sizeof(vl.host));
+	sstrncpy(vl.plugin, NFSSTAT_PLUGIN_NAME, sizeof(vl.plugin));
+	sstrncpy(vl.plugin_instance, "client",
+		sizeof(vl.plugin_instance));
+	sstrncpy(vl.type, NFSSTAT_PLUGIN_NAME, sizeof(vl.type));
+
+#if 0
+	//sstrncpy(vl.type, NFSSTAT_PLUGIN_NAME, sizeof(vl.type));
+	sstrncpy(vl.type, "client", sizeof(vl.type));
+	sstrncpy(vl.type_instance, "client", sizeof(vl.type_instance));
+#endif
+
+	plugin_dispatch_multivalue(&vl,
+		nfsstat_flags & NFSSTAT_PERCENTAGE, DS_TYPE_DERIVE,
+		"access", (derive_t) ext_nfsstats->rpccnt[NFSPROC_ACCESS],
+		"commit", (derive_t) ext_nfsstats->rpccnt[NFSPROC_COMMIT],
+		"create", (derive_t) ext_nfsstats->rpccnt[NFSPROC_CREATE],
+		"fsinfo", (derive_t) ext_nfsstats->rpccnt[NFSPROC_FSINFO],
+		"fsstat", (derive_t) ext_nfsstats->rpccnt[NFSPROC_FSSTAT],
+		"getattr", (derive_t) ext_nfsstats->rpccnt[NFSPROC_GETATTR],
+		"link", (derive_t) ext_nfsstats->rpccnt[NFSPROC_LINK],
+		"lookup", (derive_t) ext_nfsstats->rpccnt[NFSPROC_LOOKUP],
+		"mkdir", (derive_t) ext_nfsstats->rpccnt[NFSPROC_MKDIR],
+		"mknod", (derive_t) ext_nfsstats->rpccnt[NFSPROC_MKNOD],
+		"pathconf", (derive_t) ext_nfsstats->rpccnt[NFSPROC_PATHCONF],
+		"read", (derive_t) ext_nfsstats->rpccnt[NFSPROC_READ],
+		"readdir", (derive_t) ext_nfsstats->rpccnt[NFSPROC_READDIR],
+		"readirplus", (derive_t) ext_nfsstats->rpccnt[NFSPROC_READDIRPLUS],
+		"readlink", (derive_t) ext_nfsstats->rpccnt[NFSPROC_READLINK],
+		"remove", (derive_t) ext_nfsstats->rpccnt[NFSPROC_REMOVE],
+		"rename", (derive_t) ext_nfsstats->rpccnt[NFSPROC_RENAME],
+		"rmdir", (derive_t) ext_nfsstats->rpccnt[NFSPROC_RMDIR],
+		"setattr", (derive_t) ext_nfsstats->rpccnt[NFSPROC_SETATTR],
+		"symlink", (derive_t) ext_nfsstats->rpccnt[NFSPROC_SYMLINK],
+		"write", (derive_t) ext_nfsstats->rpccnt[NFSPROC_WRITE],
+		NULL
+	);
+
+#if 0
+	"TimedOut", ext_nfsstats->rpctimeouts
+	"Invalid", ext_nfsstats->rpcinvalid
+	"X Replies", ext_nfsstats->rpcunexpected
+	"Retries", ext_nfsstats->rpcretries
+	"Requests", ext_nfsstats->rpcrequests
+
+	"Attr Hits", ext_nfsstats->attrcache_hits
+	"Misses", ext_nfsstats->attrcache_misses
+	"Lookup Hits", ext_nfsstats->lookupcache_hits
+	"Misses", ext_nfsstats->lookupcache_misses
+
+	"BioR Hits", ext_nfsstats->read_bios
+	"Misses", ext_nfsstats->read_bios
+	"BioW Hits", ext_nfsstats->biocache_writes - ext_nfsstats.write_bios
+	"Misses", ext_nfsstats->write_bios
+
+	"BioRLHits", ext_nfsstats->biocache_readlinks - ext_nfsstats.readlink_bios
+	"Misses", ext_nfsstats->readlink_bios
+	"BioD Hists", ext_nfsstats->biocache_readdirs - ext_nfsstats.readdir_bios
+	"Misses", ext_nfsstats->readdir_bios
+
+	"DirE Hits", ext_nfsstats->direofcache_hits
+	"Misses", ext_nfsstats->direofcache_misses
+	"Accs Hits", ext_nfsstats->accesscache_hits
+	"Misses", ext_nfsstats->accesscache_misses
+#endif
+
+	return (0);
+}
+
+static int
+nfsstat_read(void)
+{
+	struct ext_nfsstats ext_nfsstats;
+
+	/* Get all stats for now */
+	bzero(&ext_nfsstats, sizeof(ext_nfsstats));
+	if (nfssvc(NFSSVC_GETSTATS, &ext_nfsstats) < 0) {
+		ERROR("nfssvc: Could not read NFS statistics");
+		return (-1);
+	}
+
+	if (nfsstat_flags & NFSSTAT_CLIENT_ONLY)
+		nfsstat_client_submit(&ext_nfsstats);
+
+	if (nfsstat_flags & NFSSTAT_SERVER_ONLY) {
+		struct nfsstatsv1 ext_nfsstatsv1;
+
+		/* Get all stats for now */
+		bzero(&ext_nfsstatsv1, sizeof(ext_nfsstatsv1));
+		ext_nfsstatsv1.vers = NFSSTATS_V1;
+		if (nfssvc(NFSSVC_GETSTATS | NFSSVC_NEWSTRUCT, &ext_nfsstatsv1) < 0) {
+			ERROR("nfssvc: Could not read new NFS statistics");
+			return (-1);
+		}
+
+		nfsstat_server_submit(&ext_nfsstats, &ext_nfsstatsv1);
+	}
+
+	return (0);
+}
+
+void
+module_register(void)
+{
+	plugin_register_config(NFSSTAT_PLUGIN_NAME, nfsstat_config,
+		nfsstat_config_keys, nfsstat_config_keys_size);
+	plugin_register_init(NFSSTAT_PLUGIN_NAME, nfsstat_init);
+	plugin_register_read(NFSSTAT_PLUGIN_NAME, nfsstat_read);
+} /* void module_register */
