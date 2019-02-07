# $FreeBSD$

PORTNAME=	pipewatcher
PORTVERSION=	20190206181217
PORTREVISION=	0

CATEGORIES=	freenas
VALID_CATEGORIES+=	freenas

MAINTAINER=	dev@ixsystems.com
COMMENT=	Pipe watcher to be used in replication

PRODUCT?=

USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	freenas
GH_TAGNAME=	096c782c85ca124d3d049e089039da6b1f167aa5

pre-clean:
	@

do-build:
	cd ${WRKSRC}/src/pipewatcher && ${CC} pipewatcher.c -o pipewatcher

do-install:
	mkdir -p ${STAGEDIR}${PREFIX}/bin
	${INSTALL_PROGRAM} ${WRKSRC}/src/pipewatcher/pipewatcher ${STAGEDIR}${PREFIX}/bin/

.include <bsd.port.mk>
