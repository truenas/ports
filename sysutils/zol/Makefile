# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2018122900
DISTVERSIONPREFIX=	v
CATEGORIES=	sysutils

MAINTAINER=	miwi@FreeBSD.org
COMMENT=	ZFS

LICENSE=	CDDL

RUN_DEPENDS=	zol-kmod>=0:sysutils/zol-kmod

GNU_CONFIGURE=  yes
CONFIGURE_ARGS=	--prefix=${PREFIX} --enable-debug --enable-debuginfo --with-config="user"
USES=		autoreconf:build libtool gmake
USE_GITHUB=	yes
GH_ACCOUNT=	zfsonfreebsd
GH_PROJECT=	ZoF
GH_TAGNAME=	c6d43df5b1567c1391976c4d0aad5613bad508f9

pre-configure:
	cd ${WRKSRC} && ./autogen.sh

.include <bsd.port.mk>
