# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2018122900
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
GH_TAGNAME=	c6d43df5b1567c1391976c4d0aad5613bad508f9

PLIST_FILES=	${KMODDIR}/zfs.ko

post-patch:
	${CP} ${FILESDIR}/zfs_gitrev.h ${WRKSRC}/include

do-build:
	(cd ${WRKSRC}/module/ && make -f Makefile.bsd)

do-install:
	${INSTALL} ${WRKSRC}/module/zfs.ko ${STAGEDIR}${KMODDIR}

.include <bsd.port.mk>
