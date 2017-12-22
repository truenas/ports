# $FreeBSD$

PORTNAME=	libstorj
PORTVERSION=	1.0.2
DISTVERSIONPREFIX=	v
CATEGORIES=	net

MAINTAINER=	jhixson@gmail.com
COMMENT=	C library and CLI for encrypted file transfer on the Storj network

LICENSE=	LGPL21+

LIB_DEPENDS=	libcurl.so:ftp/curl \
		libmicrohttpd.so:www/libmicrohttpd

USE_GITHUB=	yes
GH_ACCOUNT=	Storj
GH_PROJECT=	libstorj

USES=	autoreconf gmake

GNU_CONFIGURE=	yes
USE_LDCONFIG=	yes

CFLAGS+=	-I. -I${LOCALBASE}/include
LDFLAGS+=	-L. -L${LOCALBASE}/lib

.include <bsd.port.mk>
