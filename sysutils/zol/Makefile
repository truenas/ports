# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2018123000
PORTREVISION=	1
DISTVERSIONPREFIX=	v
CATEGORIES=	sysutils

MAINTAINER=	miwi@FreeBSD.org
COMMENT=	ZFS

LICENSE=	CDDL

RUN_DEPENDS=	zol-kmod>=0:sysutils/zol-kmod \

GNU_CONFIGURE=  yes
CONFIGURE_ARGS=	--prefix=${PREFIX} --enable-debug --enable-debuginfo --with-config="user"
USES=		autoreconf:build libtool gmake gettext-runtime:run
USE_GITHUB=	yes
GH_ACCOUNT=	zfsonfreebsd
GH_PROJECT=	ZoF
GH_TAGNAME=	8c2c4e23dde31e204fd6ca4cf2bb76ff5dcb735b

pre-configure:
	cd ${WRKSRC} && ./autogen.sh

.include <bsd.port.mk>
