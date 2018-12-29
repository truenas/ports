# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2018122800
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
GH_TAGNAME=	4b0b4a2806ed305245edf8375b168d977e95d701

pre-configure:
	cd ${WRKSRC} && ./autogen.sh

.include <bsd.port.mk>
