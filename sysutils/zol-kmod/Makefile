# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2019012200
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
GH_TAGNAME=	6a2a020044a42781fb3aa2faf581664ed015d39f

PLIST_FILES=	${KMODDIR}/zfs.ko

post-patch:
	${CP} ${FILESDIR}/zfs_gitrev.h ${WRKSRC}/include

do-build:
	(cd ${WRKSRC}/module/ && make -f Makefile.bsd)

do-install:
	${INSTALL} ${WRKSRC}/module/zfs.ko ${STAGEDIR}${KMODDIR}

.include <bsd.port.mk>
