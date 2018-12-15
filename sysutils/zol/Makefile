# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2018121200
DISTVERSIONPREFIX=	v
CATEGORIES=	sysutils

MAINTAINER=	miwi@FreeBSD.org
COMMENT=	ZFS

LICENSE=	CDDL

USES=		kmod
USE_GITHUB=	yes
GH_ACCOUNT=	zfsonfreebsd
GH_PROJECT=	ZoF
GH_TAGNAME=	f92b728

PLIST_FILES=	${KMODDIR}/zfs.ko

do-build:
	(cd ${WRKSRC}/module/ && make -f Makefile.bsd)

do-install:
	${INSTALL} ${WRKSRC}/module/zfs.ko ${STAGEDIR}${KMODDIR}

.include <bsd.port.mk>
