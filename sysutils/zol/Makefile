# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2019011600
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
GH_TAGNAME=	7c27faddf18b824293f1a8ed88fb61f090386dd2

pre-configure:
	cd ${WRKSRC} && ./autogen.sh

.include <bsd.port.mk>
