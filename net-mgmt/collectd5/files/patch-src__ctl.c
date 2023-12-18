--- src/ctl.c.orig	1970-01-01 01:00:00.000000000 +0100
+++ src/ctl.c	2023-12-18 22:31:12.799447885 +0100
@@ -0,0 +1,422 @@
+/**
+ * collectd - src/ctl.c
+ *
+ * Copyright (c) 2016,2018 Alexander Motin <mav@ixsystems.com>
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
+ * 3. The names of the authors may not be used to endorse or promote
+ *    products derived from this software without specific prior written
+ *    permission.
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
+#include <err.h>
+#include <unistd.h>
+#include <sys/ioctl.h>
+#include <sys/sysctl.h>
+#include <sys/time.h>
+#include <sys/types.h>
+
+#include <libxml/parser.h>
+#include <libxml/tree.h>
+#include <libxml/xpath.h>
+
+#define	CTL_BE_NAME_LEN		32
+#define	CTL_ERROR_STR_LEN	160
+
+typedef enum {
+	CTL_STATS_NO_IO,
+	CTL_STATS_READ,
+	CTL_STATS_WRITE
+} ctl_stat_types;
+#define	CTL_STATS_NUM_TYPES	3
+
+typedef enum {
+	CTL_SS_OK,
+	CTL_SS_NEED_MORE_SPACE,
+	CTL_SS_ERROR
+} ctl_stats_status;
+
+typedef enum {
+	CTL_STATS_FLAG_NONE		= 0x00,
+	CTL_STATS_FLAG_TIME_VALID	= 0x01
+} ctl_stats_flags;
+
+struct ctl_io_stats {
+	uint32_t			item;
+	uint64_t			bytes[CTL_STATS_NUM_TYPES];
+	uint64_t			operations[CTL_STATS_NUM_TYPES];
+	uint64_t			dmas[CTL_STATS_NUM_TYPES];
+	struct bintime			time[CTL_STATS_NUM_TYPES];
+	struct bintime			dma_time[CTL_STATS_NUM_TYPES];
+};
+
+struct ctl_get_io_stats {
+	struct ctl_io_stats	*stats;		/* passed to/from kernel */
+	size_t			alloc_len;	/* passed to kernel */
+	size_t			fill_len;	/* passed to userland */
+	int			first_item;	/* passed to kernel */
+	int			num_items;	/* passed to userland */
+	ctl_stats_status	status;		/* passed to userland */
+	ctl_stats_flags		flags;		/* passed to userland */
+	struct timespec		timestamp;	/* passed to userland */
+};
+
+typedef enum {
+	CTL_LUN_LIST_NONE,
+	CTL_LUN_LIST_OK,
+	CTL_LUN_LIST_NEED_MORE_SPACE,
+	CTL_LUN_LIST_ERROR
+} ctl_lun_list_status;
+
+struct ctl_lun_list {
+	char			backend[CTL_BE_NAME_LEN]; /* passed to kernel*/
+	uint32_t		alloc_len;	/* passed to kernel */
+	char                   *lun_xml;	/* filled in kernel */
+	uint32_t		fill_len;	/* passed to userland */
+	ctl_lun_list_status	status;		/* passed to userland */
+	char			error_str[CTL_ERROR_STR_LEN];
+						/* passed to userland */
+};
+
+#define	CTL_DEFAULT_DEV		"/dev/cam/ctl"
+#define	CTL_MINOR	225
+#define	CTL_PORT_LIST		_IOWR(CTL_MINOR, 0x27, struct ctl_lun_list)
+#define	CTL_GET_PORT_STATS	_IOWR(CTL_MINOR, 0x2a, struct ctl_get_io_stats)
+
+struct portstat {
+	derive_t rr, rb, wr, wb;
+	struct bintime rt, wt;
+};
+
+static void *portbuf;
+static int portbuflen = 16384;
+static struct ctl_io_stats *statbuf;
+static int ctlfd;
+
+static struct {
+	char *name;
+	char *ppvp;
+	int ha;
+	int agr;
+} *ports;
+static int maxports;
+
+static struct {
+	char *name;
+	struct portstat ps;
+} *agrs;
+static int nagrs;
+
+static int ctl_init(void)
+{
+
+	if ((ctlfd = open(CTL_DEFAULT_DEV, O_RDWR)) == -1) {
+		ERROR("cannot CTL open device %s", CTL_DEFAULT_DEV);
+		return (-1);
+	}
+	size_t maxportsize = sizeof(maxports);
+	if (sysctlbyname("kern.cam.ctl.max_ports", &maxports, &maxportsize,
+	    NULL, NULL) == -1) {
+		ERROR("cannot get CTL max ports");
+		return (-1);
+	}
+	statbuf = calloc(maxports, sizeof(struct ctl_io_stats));
+	ports = calloc(maxports, sizeof(*ports));
+	agrs = calloc(maxports, sizeof(*agrs));
+	if (statbuf == NULL || ports == NULL || agrs == NULL) {
+		ERROR("cannot allocate for %d ports", maxports);
+		free(statbuf);
+		free(ports);
+		free(agrs);
+		return (-1);
+	}
+	return (0);
+}
+
+static int ctl_shutdown(void)
+{
+
+	free(statbuf);
+	free(ports);
+	free(agrs);
+	close(ctlfd);
+	return (0);
+}
+
+static void ctl_submit (const char *type, const char *name, const char *ppvp,
+    derive_t v0, derive_t v1)
+{
+	value_t values[2];
+	value_list_t vl = VALUE_LIST_INIT;
+
+	values[0].derive = v0;
+	values[1].derive = v1;
+	vl.values = values;
+	vl.values_len = STATIC_ARRAY_SIZE (values);
+
+	sstrncpy (vl.host, hostname_g, sizeof (vl.host));
+	sstrncpy (vl.plugin, "ctl", sizeof (vl.plugin));
+	sstrncpy (vl.plugin_instance, name, sizeof (vl.plugin_instance));
+	sstrncpy (vl.type, type, sizeof (vl.type));
+	sstrncpy (vl.type_instance, ppvp, sizeof (vl.type_instance));
+
+	plugin_dispatch_values (&vl);
+}
+
+static int
+ctl_getports(void)
+{
+	struct ctl_lun_list list;
+	xmlDocPtr xml_doc;
+	xmlXPathContextPtr xpath_ctx;
+	xmlXPathObjectPtr xpath_obj;
+	int i, j, num = 0;
+
+retry:
+	if (portbuf == NULL)
+		portbuf = malloc(portbuflen);
+
+	memset(&list, 0, sizeof(list));
+	list.alloc_len = portbuflen;
+	list.lun_xml = portbuf;
+
+	if (ioctl(ctlfd, CTL_PORT_LIST, &list) == -1) {
+		ERROR("error returned from CTL_PORT_LIST ioctl");
+		return (-1);
+	}
+	if (list.status == CTL_LUN_LIST_NEED_MORE_SPACE) {
+		free(portbuf);
+		portbuf = NULL;
+		portbuflen *= 2;
+		goto retry;
+	}
+	if (list.status != CTL_LUN_LIST_OK) {
+		ERROR("bad status %d returned from CTL_PORT_LIST ioctl",
+		     list.status);
+		return (-1);
+	}
+
+	xml_doc = xmlReadDoc ((xmlChar *) list.lun_xml, NULL, NULL, XML_PARSE_NONET);
+	if (xml_doc == NULL) {
+		ERROR("xmlReadDoc error");
+		return (-1);
+	}
+	xpath_ctx = xmlXPathNewContext (xml_doc);
+	xpath_obj = xmlXPathEval((const xmlChar *) "/ctlportlist/targ_port", xpath_ctx);
+	if (xpath_obj == NULL || xpath_obj->type != XPATH_NODESET ||
+	    xpath_obj->nodesetval == NULL) {
+		ERROR("No CTL port nodes found in XML");
+		return (-1);
+	}
+
+	for (i = 0; i < maxports; i++) {
+		free(ports[i].name);
+		ports[i].name = NULL;
+		free(ports[i].ppvp);
+		ports[i].ppvp = NULL;
+		ports[i].ha = 0;
+		ports[i].agr = 0;
+		free(agrs[i].name);
+		agrs[i].name = NULL;
+	}
+	nagrs = 0;
+
+	for (i = 0; i < xpath_obj->nodesetval->nodeNr; ++i) {
+		xmlNodePtr portnode, child;
+		char *id, *name, *pp, *vp, *type, *online;
+		int pid;
+
+		portnode = xpath_obj->nodesetval->nodeTab[i];
+		if (!portnode) continue;
+		id = (char *) xmlGetProp (portnode, (xmlChar *) "id");
+		if (!id) continue;
+		pid = atoi(id);
+		xmlFree (id);
+		name = pp = vp = type = online = NULL;
+		child = portnode->xmlChildrenNode;
+		while (child != NULL) {
+			if ((!xmlStrcmp(child->name,
+			    (const xmlChar *)"port_name"))) {
+				name = (char *) xmlNodeListGetString (xml_doc,
+				    child->xmlChildrenNode, 1);
+			} else if ((!xmlStrcmp(child->name,
+			    (const xmlChar *)"physical_port"))) {
+				pp = (char *) xmlNodeListGetString (xml_doc,
+				    child->xmlChildrenNode, 1);
+			} else if ((!xmlStrcmp(child->name,
+			    (const xmlChar *)"virtual_port"))) {
+				vp = (char *) xmlNodeListGetString (xml_doc,
+				    child->xmlChildrenNode, 1);
+			} else if ((!xmlStrcmp(child->name,
+			    (const xmlChar *)"frontend_type"))) {
+				type = (char *) xmlNodeListGetString (xml_doc,
+				    child->xmlChildrenNode, 1);
+			} else if ((!xmlStrcmp(child->name,
+			    (const xmlChar *)"online"))) {
+				online = (char *) xmlNodeListGetString (xml_doc,
+				    child->xmlChildrenNode, 1);
+				if (xmlStrcmp((const xmlChar *) online, (const xmlChar *)"YES") != 0)
+					goto next;
+			}
+			child = child->next;
+		}
+		if (!name || !pp || !vp || !type)
+			goto next;
+
+		ports[pid].name = strdup(name);
+		asprintf(&ports[pid].ppvp, "%s-%s", pp, vp);
+		ports[pid].ha = (strcmp(type, "ha") == 0);
+		num++;
+
+		for (j = 0; j < nagrs; j++) {
+			if (strcmp(name, agrs[j].name) == 0)
+				break;
+		}
+		if (j >= nagrs) {
+			j = nagrs++;
+			agrs[j].name = strdup(name);
+		}
+		ports[pid].agr = j;
+
+next:
+		if (name) xmlFree (name);
+		if (pp) xmlFree(pp);
+		if (vp) xmlFree(vp);
+		if (type) xmlFree(type);
+		if (online) xmlFree(online);
+	}
+
+	xmlXPathFreeObject(xpath_obj);
+	xmlXPathFreeContext(xpath_ctx);
+	xmlFreeDoc(xml_doc);
+	return (num);
+}
+
+static int
+ctl_getstats(void)
+{
+	struct ctl_get_io_stats get_stats;
+
+	memset(&get_stats, 0, sizeof(get_stats));
+	get_stats.alloc_len = sizeof(struct ctl_io_stats) * maxports;
+	get_stats.first_item = 0;
+	memset(statbuf, 0, get_stats.alloc_len);
+	get_stats.stats = statbuf;
+
+	if (ioctl(ctlfd, CTL_GET_PORT_STATS, &get_stats) == -1) {
+		ERROR("error returned from CTL_GET_PORT_STATS ioctl");
+		return (-1);
+	}
+	if (get_stats.status != 0) {
+		ERROR("bad status %d returned from CTL_GET_PORT_STATS ioctl",
+		     get_stats.status);
+		return (-1);
+	}
+	return (get_stats.fill_len / sizeof(struct ctl_io_stats));
+}
+
+static void ps_clear(struct portstat *ps)
+{
+	ps->rr = ps->rb = ps->wr = ps->wb = 0;
+	bintime_clear(&ps->rt);
+	bintime_clear(&ps->wt);
+}
+
+static void ps_add(struct portstat *psd, struct portstat *ps)
+{
+	psd->rr += ps->rr;
+	psd->rb += ps->rb;
+	psd->wr += ps->wr;
+	psd->wb += ps->wb;
+	bintime_add(&psd->rt, &ps->rt);
+	bintime_add(&psd->wt, &ps->wt);
+}
+
+static void ps_submit(const char *name, const char *ppvp, struct portstat *ps)
+{
+	ctl_submit("disk_ops", name, ppvp, ps->rr, ps->wr);
+	ctl_submit("disk_octets", name, ppvp, ps->rb, ps->wb);
+	ctl_submit("disk_time", name, ppvp,
+	    (derive_t)ps->rt.sec*1000 + ((ps->rt.frac>>32)*1000>>32),
+	    (derive_t)ps->wt.sec*1000 + ((ps->wt.frac>>32)*1000>>32));
+}
+
+static int ctl_read (void)
+{
+	struct portstat ps;
+	struct portstat haps;
+	struct ctl_io_stats *ls;
+	int nports, nstats, port, haveha, s;
+
+	nports = ctl_getports();
+	if (nports < 0)
+		return (-1);
+	nstats = ctl_getstats();
+	if (nstats < 0)
+		return (-1);
+	for (port = 0; port < nagrs; port++)
+		ps_clear(&agrs[port].ps);
+	ps_clear(&haps);
+	haveha = 0;
+	for (port = 0; port < maxports; port++) {
+		if (ports[port].name == NULL)
+			continue;
+		for (s = 0, ls = statbuf; s < nstats; s++, ls++) {
+			if (ls->item == port)
+				break;
+		}
+		if (s >= nstats)
+			continue;
+		ps_clear(&ps);
+		ps.rr += ls->operations[CTL_STATS_NO_IO];
+		bintime_add(&ps.rt, &ls->time[CTL_STATS_NO_IO]);
+		ps.rr += ls->operations[CTL_STATS_READ];
+		bintime_add(&ps.rt, &ls->time[CTL_STATS_READ]);
+		ps.rb += ls->bytes[CTL_STATS_READ];
+		ps.wr += ls->operations[CTL_STATS_WRITE];
+		ps.wb += ls->bytes[CTL_STATS_WRITE];
+		bintime_add(&ps.wt, &ls->time[CTL_STATS_WRITE]);
+		ps_submit(ports[port].name, ports[port].ppvp, &ps);
+		ps_add(&agrs[ports[port].agr].ps, &ps);
+		if (ports[port].ha) {
+			ps_add(&haps, &ps);
+			haveha = 1;
+		}
+	}
+	for (port = 0; port < nagrs; port++)
+		ps_submit(agrs[port].name, "", &agrs[port].ps);
+	if (haveha)
+		ps_submit("ha", "", &haps);
+	return (0);
+}
+
+void module_register (void)
+{
+	plugin_register_init ("ctl", ctl_init);
+	plugin_register_shutdown ("ctl", ctl_shutdown);
+	plugin_register_read ("ctl", ctl_read);
+} /* void module_register */
