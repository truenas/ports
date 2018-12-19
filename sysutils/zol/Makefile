# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2018121700
DISTVERSIONPREFIX=	v
CATEGORIES=	sysutils

MAINTAINER=	miwi@FreeBSD.org
COMMENT=	ZFS

LICENSE=	CDDL

RUN_DEPENDS=	zol-kmod>=0:sysutils/zol-kmod

GNU_CONFIGURE=  yes
CONFIGURE_ARGS=	--prefix=${PREFIX} --with-config="user"
USES=		autoreconf:build libtool gmake
USE_GITHUB=	yes
GH_ACCOUNT=	zfsonfreebsd
GH_PROJECT=	ZoF
GH_TAGNAME=	10f2d4b

pre-configure:
	cd ${WRKSRC} && ./autogen.sh

.include <bsd.port.mk>
