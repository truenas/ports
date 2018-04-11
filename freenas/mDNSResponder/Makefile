# Created by: Alfred Perlstein <alfred@FreeBSD.org>
# $FreeBSD$

PORTNAME=	mDNSResponder
PORTVERSION=	878.1.1
PORTREVISION=	0
CATEGORIES=	freenas net dns
MASTER_SITES=   https://api.github.com/repos/freenas/mDNSResponder/tarball/
DISTNAME=       freenas/master

MAINTAINER=	john@ixsystems.com
COMMENT=	Bonjour (zero-configuration networking) by Apple

LICENSE=	APACHE20 BSD3CLAUSE
LICENSE_COMB=	dual

EXTRACT_SUFX=
NO_CHECKSUM=	yes
DISABLE_SIZE=	yes

FREENAS_DISTDIR=	${DISTDIR}/freenas

GH_BRANCH?=	${DISTNAME}
GH_FILE=	${GH_BRANCH:S/freenas\///}
GH_TARBALL=	${GH_FILE}.tar.gz

DISTFILES=	${DISTNAME}

MDNS_DIR=	${GH_BRANCH:S/freenas\///}
WRKSRC?=	${WRKDIR}/${MDNS_DIR}

# lib/libdns_sd.so.1 sbin/mdnsd
CONFLICTS_INSTALL=	avahi-libdns openmdns

OPTIONS_DEFINE=	IPV6

ALL_TARGET=	all dnsextd
BUILD_WRKSRC=	${WRKSRC}/mDNSPosix
INSTALL_WRKSRC=	${WRKSRC}/mDNSPosix
MAKE_ARGS=	LDCONFIG=${TRUE} LOCALBASE=${PREFIX} MAN_PATH=${MANPREFIX}/man STRIP=${STRIP_CMD} os=freebsd
MAKE_JOBS_UNSAFE=	yes
USE_LDCONFIG=	yes
USE_RC_SUBR=	mdnsd mdnsresponderposix
USES=		cpe gmake

CPE_VENDOR=	apple
CPE_PRODUCT=	${PORTNAME:tl}

EXTRA_BIN=	mDNSClientPosix mDNSIdentify mDNSNetMonitor mDNSProxyResponderPosix mDNSResponderPosix
EXTRA_MAN1=	dns-sd.1
EXTRA_MAN8=	dnsextd.8 mDNSResponderPosix.8
EXTRA_SBIN=	dnsextd

# Ewww, but Yeesss
pre-everything::
	rm -rf ${FREENAS_DISTDIR}/${GH_FILE} ${FREENAS_DISTDIR}/${GH_TARBALL}

pre-extract:
	${MV} ${FREENAS_DISTDIR}/${GH_FILE} ${FREENAS_DISTDIR}/${GH_TARBALL}

do-extract: ${EXTRACT_WRKDIR}
	if ! (cd ${EXTRACT_WRKDIR} && tar -zxvf ${FREENAS_DISTDIR}/${GH_TARBALL} --no-same-owner --no-same-permissions);\
	then \
		exit 1; \
	fi; \
	if [ ${UID} = 0 ]; then \
		${CHMOD} -R ug-s ${WRKDIR}; \
		${CHOWN} -R 0:0 ${WRKDIR}; \
	fi; \
	if [ -d ${EXTRACT_WRKDIR}/${MDNS_DIR} ]; then \
		${ECHO_CMD} "${EXTRACT_WRKDIR}/${MDNS_DIR} exists, please run 'make clean' first"; \
		exit 1; \
	fi; \
	${MV} ${EXTRACT_WRKDIR}/freenas-mDNSResponder-* "${EXTRACT_WRKDIR}/${MDNS_DIR}"

post-patch:
	@${REINPLACE_CMD} -e '/^CFLAGS_DEBUG/ s|-Os|${CFLAGS}|' ${WRKSRC}/mDNSPosix/Makefile
	@${REINPLACE_CMD} -e 's|\(#define IPV6_2292_PKTINFO \).*|\1IPV6_PKTINFO|; s|\(#define IPV6_2292_HOPLIMIT \).*|\1IPV6_HOPLIMIT|' ${WRKSRC}/mDNSPosix/mDNSUNP.h
	@${CP} ${WRKSRC}/mDNSShared/mDNSResponder.8 ${WRKSRC}/mDNSShared/mDNSResponderPosix.8

post-patch-IPV6-off:
	@${REINPLACE_CMD} -e '/^CFLAGS_OS/ s|-DHAVE_IPV6|-UHAVE_IPV6|' ${WRKSRC}/mDNSPosix/Makefile

post-install:
	cd ${WRKSRC}/mDNSPosix/build/prod/ && ${INSTALL_PROGRAM} ${EXTRA_BIN} ${STAGEDIR}${PREFIX}/bin/
	cd ${WRKSRC}/mDNSShared/ && ${INSTALL_MAN} ${EXTRA_MAN1} ${STAGEDIR}${PREFIX}/man/man1/
	cd ${WRKSRC}/mDNSShared/ && ${INSTALL_MAN} ${EXTRA_MAN8} ${STAGEDIR}${PREFIX}/man/man8/
	cd ${WRKSRC}/mDNSPosix/build/prod/ && ${INSTALL_PROGRAM} ${EXTRA_SBIN} ${STAGEDIR}${PREFIX}/sbin/
	${STRIP_CMD} ${STAGEDIR}${PREFIX}/bin/dns-sd

.include <bsd.port.mk>
