# $FreeBSD$

PORTNAME=	pipewatcher
PORTVERSION=	20181219152913
PORTREVISION=	0

CATEGORIES=	freenas
VALID_CATEGORIES+=	freenas

MAINTAINER=	dev@ixsystems.com
COMMENT=	Pipe watcher to be used in replication

PRODUCT?=

USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	freenas
GH_TAGNAME=	cd0aa52bfbe74e7341c4a552a5f49e0cc957b66e

pre-clean:
	@

do-build:
	cd ${WRKSRC}/src/pipewatcher && ${CC} pipewatcher.c -o pipewatcher

do-install:
	mkdir -p ${STAGEDIR}${PREFIX}/bin
	${INSTALL_PROGRAM} ${WRKSRC}/src/pipewatcher/pipewatcher ${STAGEDIR}${PREFIX}/bin/

.include <bsd.port.mk>
