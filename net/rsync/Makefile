# Created by: David O'Brien (obrien@cs.ucdavis.edu)
# $FreeBSD$

PORTNAME=	rsync
<<<<<<< HEAD
PORTVERSION=	3.1.1
PORTREVISION=	4
=======
PORTVERSION=	3.0.9
PORTREVISION=	3
>>>>>>> freenas/9.2.1-BRANCH
CATEGORIES=	net ipv6
MASTER_SITES=	http://rsync.samba.org/ftp/%SUBDIR%/ \
		ftp://ftp.samba.org/pub/%SUBDIR%/ \
		ftp://ftp.sunet.se/pub/unix/admin/%SUBDIR%/  \
		ftp://ftp.fu-berlin.de/pub/unix/network/%SUBDIR%/ \
		http://www.mirrorservice.org/sites/rsync.samba.org/ \
		CRITICAL
MASTER_SITE_SUBDIR=	rsync
DISTFILES=	${DISTNAME}${EXTRACT_SUFX}

MAINTAINER=	ehaupt@FreeBSD.org
COMMENT=	Network file distribution/synchronization utility

LICENSE=	GPLv3

USES=		cpe

CPE_VENDOR=	samba
CPE_PRODUCT=	rsync

PATCH_STRIP=	-p1
GNU_CONFIGURE=	yes
USE_RC_SUBR=	rsyncd
SUB_LIST=	NAME=rsyncd

INSTALL_TARGET=	install-strip

CONFIGURE_ARGS=	--disable-debug --enable-ipv6 \
		--with-rsyncd-conf=${ETCDIR}/rsyncd.conf

PORTDOCS=	NEWS README csprotocol.txt tech_report.tex

<<<<<<< HEAD
# define options
OPTIONS_DEFINE=		DOCS POPT_PORT ZLIB_BASE SSH ICONV
OPTIONS_RADIO=		PTS
OPTIONS_RADIO_PTS=	TIMELIMIT RENAMED FLAGS ATIMES ACL
=======
MAN1=		rsync.1
MAN5=		rsyncd.conf.5
>>>>>>> freenas/9.2.1-BRANCH

OPTIONS_DEFINE=	POPT_PORT SSH FLAGS ATIMES ACL ICONV TIMELIMIT RENAMED DOCS
POPT_PORT_DESC=	Use popt from devel/popt instead of bundled one
SSH_DESC=	Use SSH instead of RSH
FLAGS_DESC=	File system flags support patch, adds --fileflags
ATIMES_DESC=	Preserve access times, adds --atimes
ACL_DESC=	Add backward-compatibility for the --acls option
TIMELIMIT_DESC=	Time limit patch
RENAMED_DESC=	Add support for renamed file detection

<<<<<<< HEAD
# define default options
OPTIONS_DEFAULT=SSH FLAGS

ZLIB_BASE_CONFIGURE_ON=	--with-included-zlib=no
TIMELIMIT_EXTRA_PATCHES=	${WRKSRC}/patches/time-limit.diff
ATIMES_EXTRA_PATCHES=		${WRKSRC}/patches/atimes.diff
FLAGS_EXTRA_PATCHES=		${WRKSRC}/patches/fileflags.diff \
				${FILESDIR}/extrapatch-main.c
RENAMED_EXTRA_PATCHES=		${WRKSRC}/patches/detect-renamed.diff
ACL_EXTRA_PATCHES=		${WRKSRC}/patches/acls.diff \
				${FILESDIR}/extrapatch-acl

ICONV_USES=	iconv
ICONV_CPPFLAGS=	-I${LOCALBASE}/include
ICONV_LDFLAGS=	-L${LOCALBASE}/lib
ICONV_CONFIGURE_ENABLE=	iconv iconv-open

POPT_PORT_LIB_DEPENDS=	libpopt.so:${PORTSDIR}/devel/popt
POPT_PORT_CPPFLAGS=	-I${LOCALBASE}/include
POPT_PORT_LDFLAGS=	-L${LOCALBASE}/lib
POPT_CONFIGURE_WITH=	included-popt

SSH_CONFIGURE_ON=	--with-rsh=ssh
SSH_CONFIGURE_OFF=	--with-rsh=rsh
=======
OPTIONS_DEFAULT=SSH
>>>>>>> freenas/9.2.1-BRANCH

NO_STAGE=	yes
.include <bsd.port.options.mk>

<<<<<<< HEAD
=======
.if ${PORT_OPTIONS:MTIMELIMIT}
PATCH_STRIP=	-p1
EXTRA_PATCHES+=	${WRKSRC}/patches/time-limit.diff
.if ${PORT_OPTIONS:MFLAGS}
IGNORE=		flags and timelimit can't be enabled simultaneously. Please rerun 'make config' and disable one of them
.endif
.if ${PORT_OPTIONS:MATIMES}
IGNORE=		atimes and timelimit can't be enabled simultaneously. Please rerun 'make config' and disable one of them
.endif
.endif

.if ${PORT_OPTIONS:MATIMES}
PATCH_STRIP=	-p1
EXTRA_PATCHES+=	${WRKSRC}/patches/atimes.diff
.if ${PORT_OPTIONS:MFLAGS}
IGNORE=		flags and atimes can't be enabled simultaneously. Please rerun 'make config' and disable one of them
.endif
.endif

>>>>>>> freenas/9.2.1-BRANCH
.if ${PORT_OPTIONS:MICONV} || ${PORT_OPTIONS:MFLAGS} || ${PORT_OPTIONS:MACL} || make(makesum) || ${PORT_OPTIONS:MTIMELIMIT} || ${PORT_OPTIONS:MATIMES} || ${PORT_OPTIONS:MRENAMED}
DISTFILES+=	${PORTNAME}-patches-${PORTVERSION}${EXTRACT_SUFX}
.endif

<<<<<<< HEAD
.include <bsd.port.pre.mk>

.if empty(ICONV_LIB)
CONFIGURE_ARGS+=ac_cv_search_libiconv_open=no
=======
.if ${PORT_OPTIONS:MFLAGS}
PATCH_STRIP=	-p1
EXTRA_PATCHES+=	${WRKSRC}/patches/fileflags.diff
# https://bugzilla.samba.org/show_bug.cgi?id=8941
EXTRA_PATCHES+=	${FILESDIR}/extrapatch-syscall.c
.endif

.if ${PORT_OPTIONS:MRENAMED}
PATCH_STRIP=	-p1
#EXTRA_PATCHES+=	${WRKSRC}/patches/detect-renamed.diff
EXTRA_PATCHES+=	${FILESDIR}/extrapatch-detect-renamed.diff
>>>>>>> freenas/9.2.1-BRANCH
.endif

post-patch:
<<<<<<< HEAD
	@${REINPLACE_CMD} -e 's|perl.*|${DO_NADA}|' ${WRKSRC}/Makefile.in
	@${REINPLACE_CMD} -e 's:/etc/:${ETCDIR}/:g'  \
=======
	@${REINPLACE_CMD} -e 's|CFLAGS|CONFIGUREDCFLAGS|' \
		-e 's|perl.*|${DO_NADA}|' \
			${WRKSRC}/Makefile.in
# workaround for bug in configure which ignores --with-rsyncd-conf 
	@${REINPLACE_CMD} -e 's|RSYNCD_SYSCONF|"${PREFIX}/etc/rsyncd.conf"|' \
			${WRKSRC}/clientserver.c

pre-configure:
	@${REINPLACE_CMD} -e 's:/etc/:${PREFIX}/etc/:g'  \
>>>>>>> freenas/9.2.1-BRANCH
		${WRKSRC}/rsync.h ${WRKSRC}/rsync.1 ${WRKSRC}/rsyncd.conf.5

post-install:
<<<<<<< HEAD
	@${MKDIR} ${STAGEDIR}${ETCDIR}
	${INSTALL_DATA} ${FILESDIR}/rsyncd.conf.sample ${STAGEDIR}${ETCDIR}/
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
	${INSTALL_DATA} ${PORTDOCS:S,^,${WRKSRC}/,} ${STAGEDIR}${DOCSDIR}
=======
	@${STRIP_CMD} ${PREFIX}/bin/rsync
	@${INSTALL_DATA} ${FILESDIR}/rsyncd.conf.sample ${PREFIX}/etc/
	@[ -f ${PREFIX}/etc/rsyncd.conf ] || \
		${CP} ${PREFIX}/etc/rsyncd.conf.sample \
		${PREFIX}/etc/rsyncd.conf
.if ${PORT_OPTIONS:MDOCS}
	@${MKDIR} ${DOCSDIR}
	@${INSTALL_DATA} ${PORTDOCS:S,^,${WRKSRC}/,} ${DOCSDIR}
.endif
>>>>>>> freenas/9.2.1-BRANCH

test: build
	@(cd ${WRKSRC} && ${MAKE_CMD} check)

.include <bsd.port.post.mk>
