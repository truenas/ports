# $FreeBSD$

PORTNAME=	pipewatcher
PORTVERSION=	20190610174617
PORTREVISION=	0

CATEGORIES=	freenas
VALID_CATEGORIES+=	freenas

MAINTAINER=	dev@ixsystems.com
COMMENT=	Pipe watcher to be used in replication

PRODUCT?=

USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	freenas
GH_TAGNAME=	df3851a5a3d4c86e509ea53ddf7a438732b5c0d9

pre-clean:
	@

do-build:
	cd ${WRKSRC}/src/pipewatcher && ${CC} pipewatcher.c -o pipewatcher

do-install:
	mkdir -p ${STAGEDIR}${PREFIX}/bin
	${INSTALL_PROGRAM} ${WRKSRC}/src/pipewatcher/pipewatcher ${STAGEDIR}${PREFIX}/bin/

.include <bsd.port.mk>
