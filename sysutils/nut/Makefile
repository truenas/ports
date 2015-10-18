# Created by: Boris Popov <bp@FreeBSD.org>
# $FreeBSD: head/sysutils/nut/Makefile 327772 2013-09-20 23:05:58Z bapt $

PORTNAME=	nut
<<<<<<< HEAD
PORTVERSION=	2.7.3
=======
PORTVERSION=	2.7.1
>>>>>>> freenas/9.2.1-BRANCH
CATEGORIES=	sysutils
MASTER_SITES=	http://www.networkupstools.org/source/${PORTVERSION:R}/
DISTFILES=	${DISTNAME}${EXTRACT_SUFX} ${DISTNAME}${EXTRACT_SUFX}.sig
EXTRACT_ONLY=	${DISTNAME}${EXTRACT_SUFX}

MAINTAINER=	cy@FreeBSD.org
COMMENT=	Network UPS Tools

USE_AUTOTOOLS=	autoconf
GNU_CONFIGURE=	yes
USE_LDCONFIG=	yes
USES=		gmake pkgconfig libtool

NUT_USER?=	uucp
NUT_GROUP?=	uucp
STATEDIR?=	/var/db/nut
PLIST_SUB+=	NUT_USER=${NUT_USER}
PLIST_SUB+=	NUT_GROUP=${NUT_GROUP}

OPTIONS_DEFAULT=SERIAL USB SNMP NEON IPMI_OFF
OPTIONS_DEFINE=	SERIAL USB SNMP NEON PDU CGI BASH AVAHI OPENSSL DOCS
OPTIONS_SINGLE=	IPMI
OPTIONS_SINGLE_IPMI=	IPMI_OFF IPMIPSU FREEIPMI

SERIAL_DESC=	SERIAL support
USB_DESC=	USB support
SNMP_DESC=	SNMP support
NEON_DESC=	NEON XML/HTTP support
PDU_DESC=	Powerman PDU support
CGI_DESC=	Web CGI interface
BASH_DESC=	Bash Completion support
AVAHI_DESC=	Avahi support
IPMI_OFF_DESC=	No IPMI support
FREEIPMI_DESC=	freeipmi support
IPMIPSU_DESC=	Use nut-ipmipsu support (experimental)

USE_RC_SUBR=	nut nut_upsmon nut_upslog
SUB_LIST+=	STATEDIR=${STATEDIR}
PLIST_SUB+=	STATEDIR=${STATEDIR}

<<<<<<< HEAD
=======
MAN3=		nutscan.3 nutscan_add_device_to_device.3 \
		nutscan_add_option_to_device.3 nutscan_cidr_to_ip.3 \
		nutscan_display_parsable.3 nutscan_display_ups_conf.3 \
		nutscan_free_device.3 nutscan_init.3 nutscan_new_device.3 \
		nutscan_scan_avahi.3 nutscan_scan_ipmi.3 nutscan_scan_nut.3 \
		nutscan_scan_snmp.3 nutscan_scan_usb.3 nutscan_scan_xml_http.3 \
		upscli_connect.3 upscli_disconnect.3 upscli_fd.3 \
		upscli_get.3 upscli_list_next.3 upscli_list_start.3 \
		upscli_readline.3 upscli_sendline.3 upscli_splitaddr.3 \
		upscli_splitname.3 upscli_ssl.3 upscli_strerror.3 \
		upscli_upserror.3 upsclient.3 \
		libnutclient.3 libnutclient_commands.3 libnutclient_devices.3 \
		libnutclient_general.3 libnutclient_misc.3 libnutclient_tcp.3 \
		libnutclient_variables.3 nutclient_authenticate.3 \
		nutclient_destroy.3 nutclient_device_forced_shutdown.3 \
		nutclient_device_login.3 nutclient_device_master.3 \
		nutclient_execute_device_command.3 \
		nutclient_get_device_command_description.3 \
		nutclient_get_device_commands.3 \
		nutclient_get_device_description.3 \
		nutclient_get_device_num_logins.3 \
		nutclient_get_device_rw_variables.3 \
		nutclient_get_device_variable_description.3 \
		nutclient_get_device_variable_values.3 \
		nutclient_get_device_variables.3 nutclient_get_devices.3 \
		nutclient_has_device.3 nutclient_has_device_command.3 \
		nutclient_has_device_variable.3 nutclient_logout.3 \
		nutclient_set_device_variable_value.3 \
		nutclient_set_device_variable_values.3 \
		nutclient_tcp_create_client.3 nutclient_tcp_disconnect.3 \
		nutclient_tcp_get_timeout.3 nutclient_tcp_is_connected.3 \
		nutclient_tcp_reconnect.3 nutclient_tcp_set_timeout.3 \
		nutscan_get_serial_ports_list.3 nutscan_scan_eaton_serial.3 \
		upscli_add_host_cert.3 upscli_cleanup.3 upscli_init.3

MAN5=		nut.conf.5 ups.conf.5 upsd.conf.5 upsd.users.5 upsmon.conf.5 \
		upssched.conf.5

MAN8=		nut-recorder.8 nut-scanner.8 nutupsdrv.8 upsc.8 \
		upscmd.8 upsd.8 upsdrvctl.8 upslog.8 upsmon.8 upsrw.8 upssched.8

>>>>>>> freenas/9.2.1-BRANCH
PORTDOCS=	*

.include <bsd.port.options.mk>

CONFIGURE_ARGS=	--sysconfdir=${PREFIX}/etc/nut \
		--program-transform-name="" \
		--localstatedir=${STATEDIR} \
		--datadir=${PREFIX}/etc/nut \
		--with-devd-dir=${PREFIX}/etc/devd \
		--with-drvpath=${PREFIX}/libexec/nut \
		--with-statepath=${STATEDIR} \
		--with-altpidpath=${STATEDIR} \
		--with-pidpath=${STATEDIR} \
		--with-pkgconfig-dir=${PREFIX}/libdata/pkgconfig \
		--with-user=${NUT_USER} \
		--with-group=${NUT_GROUP} \
		--with-dev

.if ${PORT_OPTIONS:MCGI}
LIB_DEPENDS+=	libgd.so:${PORTSDIR}/graphics/gd
CGIDIR?=	${PREFIX}/www/cgi-bin/${PORTNAME}
CGIDIR_REL?=	${CGIDIR:S,^${PREFIX}/,,}
CONFIGURE_ARGS+=	--with-cgi --with-cgipath=${CGIDIR} \
			--with-htmlpath=${WWWDIR} \
			--with-gd-includes=-I${LOCALBASE}/include \
			--with-gd-libs="-L${LOCALBASE}/lib -lgd"
PLIST_SUB+=	NUT_CGI=""
PLIST_SUB+=	CGIDIR="${CGIDIR_REL}"
PLIST_SUB+=	CGIETCDIR="etc/nut/"
.else
CONFIGURE_ARGS+=	--without-cgi
PLIST_SUB+=	NUT_CGI="@comment "
.endif

.if ${PORT_OPTIONS:MSERIAL}
CONFIGURE_ARGS+=	--with-serial
<<<<<<< HEAD
=======
MAN8+=		apcsmart.8 apcsmart-old.8 bestfortress.8 bcmxcp.8 belkin.8 \
		belkinunv.8 bestuferrups.8 bestups.8 bestfcom.8 \
		clone.8 dummy-ups.8 etapro.8 everups.8 gamatronic.8 \
		genericups.8 isbmex.8 ivtscd.8 liebert.8 liebert-esp2.8 \
		masterguard.8 metasys.8 mge-shut.8 mge-utalk.8 microdowell.8 \
		oneac.8 optiups.8 powercom.8 powerpanel.8 rhino.8 safenet.8 \
		solis.8 tripplite.8 tripplitesu.8 upscode2.8 victronups.8 \
		blazer_ser.8 riello_ser.8 al175.8 apcupsd-ups.8 nutdrv_qx.8
>>>>>>> freenas/9.2.1-BRANCH
PLIST_SUB+=	NUT_SERIAL=""
.else
CONFIGURE_ARGS+=	--without-serial
PLIST_SUB+=	NUT_SERIAL="@comment "
.endif

.if ${PORT_OPTIONS:MUSB}
#EXTRA_PATCHES=	${FILESDIR}/extra-patch-m4_nut_check_libusb.m4
CONFIGURE_ARGS+=	--with-usb=auto
<<<<<<< HEAD
=======
MAN8+=		bcmxcp_usb.8 richcomm_usb.8 tripplite_usb.8 usbhid-ups.8 \
		blazer_usb.8 riello_usb.8
>>>>>>> freenas/9.2.1-BRANCH
PLIST_SUB+=	NUT_USB=""
.else
CONFIGURE_ARGS+=	--without-usb
PLIST_SUB+=	NUT_USB="@comment "
.endif

.if ${PORT_OPTIONS:MSNMP}
LIB_DEPENDS+=	libnetsnmp.so:${PORTSDIR}/net-mgmt/net-snmp
CONFIGURE_ARGS+=	--with-snmp
PLIST_SUB+=	NUT_SNMP=""
.else
CONFIGURE_ARGS+=	--without-snmp
PLIST_SUB+=	NUT_SNMP="@comment "
.endif

.if ${PORT_OPTIONS:MNEON}
LIB_DEPENDS+=	libneon.so:${PORTSDIR}/www/neon29
CONFIGURE_ARGS+=	--with-neonxml
PLIST_SUB+=	NUT_NEON=""
.else
CONFIGURE_ARGS+=	--without-neonxml
CONFIGURE_ARGS+=	--disable-neonxml
PLIST_SUB+=	NUT_NEON="@comment "
.endif

.if ${PORT_OPTIONS:MPDU}
LIB_DEPENDS+=	libpowerman.so:${PORTSDIR}/sysutils/powerman
CONFIGURE_ARGS+=	--with-powerman
PLIST_SUB+=	NUT_PDU=""
.else
CONFIGURE_ARGS+=	--without-powerman
PLIST_SUB+=	NUT_PDU="@comment "
.endif

.if ${PORT_OPTIONS:MBASH}
RUN_DEPENDS+=	${LOCALBASE}/share/bash-completion/bash_completion.sh:${PORTSDIR}/shells/bash-completion
PLIST_SUB+=	BASH=""
.else
PLIST_SUB+=	BASH="@comment "
.endif

.if ${PORT_OPTIONS:MAVAHI}
BUILD_DEPENDS+=	${LOCALBASE}/include/avahi-ui/avahi-ui.h:${PORTSDIR}/net/avahi-header
RUN_DEPENDS+=	avahi-daemon:${PORTSDIR}/net/avahi-app
CONFIGURE_ARGS+=	--with-avahi
.else
CONFIGURE_ARGS+=	--without-avahi
.endif

.if ${PORT_OPTIONS:MFREEIPMI}
LIB_DEPENDS+=	libfreeipmi.so:${PORTSDIR}/sysutils/freeipmi
CONFIGURE_ARGS+=	--with-freeipmi
PLIST_SUB+=	NUT_FREEIPMI=""
.else
CONFIGURE_ARGS+=	--without-freeipmi
PLIST_SUB+=	NUT_FREEIPMI="@comment "
.endif

.if ${PORT_OPTIONS:MIPMIPSU}
CONFIGURE_ARGS+=	--with-ipmi
PLIST_SUB+=	NUT_IPMIPSU=""
.else
CONFIGURE_ARGS+=	--without-ipmi
PLIST_SUB+=	NUT_IPMIPSU="@comment "
.endif

.if ${PORT_OPTIONS:MOPENSSL}
USE_OPENSSL=	yes
CONFIGURE_ARGS+=	--with-openssl
.else
CONFIGURE_ARGS+=	--without-openssl --without-ssl
.endif

pre-install:
	@${MKDIR} ${STAGEDIR}${PREFIX}/libexec/nut
	@${REINPLACE_CMD} -e 's/device-name\*/cdev/g' ${WRKSRC}/scripts/devd/nut-usb.conf

.if ${PORT_OPTIONS:MDOCS}
post-install:
.if ${PORT_OPTIONS:MBASH}
	@${MKDIR} ${STAGEDIR}${PREFIX}/etc/bash_completion.d/
	${INSTALL_DATA} ${WRKSRC}/scripts/misc/nut.bash_completion ${STAGEDIR}${PREFIX}/etc/bash_completion.d/
.endif
	@${MKDIR} ${STAGEDIR}${DOCSDIR}/cables
	${INSTALL_DATA} ${WRKSRC}/docs/cables/*.txt ${STAGEDIR}${DOCSDIR}/cables
	${INSTALL_DATA} ${WRKSRC}/docs/*.txt ${STAGEDIR}${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/docs/FAQ.txt ${STAGEDIR}${DOCSDIR}
.for file in AUTHORS COPYING ChangeLog INSTALL MAINTAINERS NEWS README UPGRADING
	${INSTALL_DATA} ${WRKSRC}/${file} ${STAGEDIR}${DOCSDIR}
.endfor
.endif

.include <bsd.port.mk>
