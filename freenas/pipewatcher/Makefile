# $FreeBSD$

PORTNAME=	pipewatcher
PORTVERSION=	20181210151436
PORTREVISION=	0

CATEGORIES=	freenas
VALID_CATEGORIES+=	freenas

MAINTAINER=	dev@ixsystems.com
COMMENT=	Pipe watcher to be used in replication

PRODUCT?=

USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	freenas
GH_TAGNAME=	81f00e1cf38def4af85915d4d9c45a163b3a042c

pre-clean:
	@

do-build:
	cd ${WRKSRC}/src/pipewatcher && ${CC} pipewatcher.c -o pipewatcher

do-install:
	mkdir -p ${STAGEDIR}${PREFIX}/bin
	${INSTALL_PROGRAM} ${WRKSRC}/src/pipewatcher/pipewatcher ${STAGEDIR}${PREFIX}/bin/

.include <bsd.port.mk>
