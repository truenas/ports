# $FreeBSD$

PORTNAME=	openzfs
PORTVERSION=	2020030303
DISTVERSIONPREFIX=	v
CATEGORIES=	sysutils
PKGNAMESUFFIX?=	-kmod

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	OpenZFS kernel module for FreeBSD

LICENSE=	CDDL

USES=		kmod
USE_GITHUB=	yes
GH_ACCOUNT=	zfsonfreebsd
GH_PROJECT=	ZoF
GH_TAGNAME=	0551fe102
KMODTARGET?=	openzfs.ko

PLIST_FILES=	${KMODDIR}/${KMODTARGET}
PLIST_FILES+=	${KMODDIR}/${KMODTARGET}.debug

OPTIONS_DEFINE=	DEBUG GCOV
OPTIONS_SUB=	yes
GCOV_DESC=	Build kernel module with GCOV support (Requires gcc)

.include <bsd.port.pre.mk>

.if (${OSVERSION} < 1200504) || (${OSVERSION} >= 1300000 && ${OSVERSION} < 1300013)
IGNORE=      needs FreeBSD 12/13 with AES-CCM support
.endif

.if ${PORT_OPTIONS:MDEBUG}
MAKE_OPTS+=	WITH_DEBUG=true
.endif

.if ${PORT_OPTIONS:MGCOV}
USE_GCC=yes
MAKE_OPTS+=	WITH_GCOV=true
.endif

ZFS_META_VERSION=	${DISTVERSIONPREFIX}${PORTVERSION}
ZFS_META_RELEASE=	${GH_PROJECT}_${GH_TAGNAME}

post-patch:
	${CP} ${FILESDIR}/zfs_gitrev.h ${WRKSRC}/include
	${ECHO} '#define ZFS_META_VERSION "${ZFS_META_VERSION}"' > ${WRKSRC}/zfs_config.h
	${ECHO} '#define ZFS_META_RELEASE "${ZFS_META_RELEASE}"' >> ${WRKSRC}/zfs_config.h

do-build:
	(cd ${WRKSRC}/module/ && make -f Makefile.bsd CC=${CC} ${MAKE_OPTS})

do-install:
	${INSTALL} ${WRKSRC}/module/openzfs.ko ${STAGEDIR}${KMODDIR}/${KMODTARGET}
	${INSTALL} ${WRKSRC}/module/openzfs.ko.debug ${STAGEDIR}${KMODDIR}/${KMODTARGET}.debug

.include <bsd.port.post.mk>
