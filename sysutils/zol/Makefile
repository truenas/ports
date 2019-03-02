# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2019030100
DISTVERSIONPREFIX=	v
CATEGORIES=	sysutils

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	ZFS

LICENSE=	CDDL

RUN_DEPENDS=	zol-kmod>=0:sysutils/zol-kmod

USES=		autoreconf:build libtool gmake gettext-runtime:run

GNU_CONFIGURE=  yes
CONFIGURE_ARGS=	--prefix=${PREFIX} --enable-debug --enable-debuginfo --with-config="user"

USE_GITHUB=	yes
GH_ACCOUNT=	zfsonfreebsd
GH_PROJECT=	ZoF
GH_TAGNAME=	e10a5fb

pre-configure:
	cd ${WRKSRC} && ./autogen.sh

.include <bsd.port.mk>
