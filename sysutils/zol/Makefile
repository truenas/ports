# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2019011100
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
GH_TAGNAME=	9c790d2668e23575dca6a7376e5e8179c4e62846

pre-configure:
	cd ${WRKSRC} && ./autogen.sh

.include <bsd.port.mk>
