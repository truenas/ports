# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2018122800
PORTREVISION=	1
DISTVERSIONPREFIX=	v
CATEGORIES=	sysutils
PKGNAMESUFFIX=	-kmod

MAINTAINER=	miwi@FreeBSD.org
COMMENT=	ZFS

LICENSE=	CDDL

USES=		kmod
USE_GITHUB=	yes
GH_ACCOUNT=	zfsonfreebsd
GH_PROJECT=	ZoF
GH_TAGNAME=	4b0b4a2806ed305245edf8375b168d977e95d701

PLIST_FILES=	${KMODDIR}/zfs.ko

post-patch:
	${CP} ${FILESDIR}/zfs_gitrev.h ${WRKSRC}/include

do-build:
	(cd ${WRKSRC}/module/ && make -f Makefile.bsd)

do-install:
	${INSTALL} ${WRKSRC}/module/zfs.ko ${STAGEDIR}${KMODDIR}

.include <bsd.port.mk>
