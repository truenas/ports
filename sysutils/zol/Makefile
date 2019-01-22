# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2019012200
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
GH_TAGNAME=	6a2a020044a42781fb3aa2faf581664ed015d39f

pre-configure:
	cd ${WRKSRC} && ./autogen.sh

.include <bsd.port.mk>
