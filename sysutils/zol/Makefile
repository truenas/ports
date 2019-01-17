# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2019011700
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
GH_TAGNAME=	e620e015cb94f6dc690c0c674c1a296a224a1724

pre-configure:
	cd ${WRKSRC} && ./autogen.sh

.include <bsd.port.mk>
