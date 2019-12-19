# $FreeBSD$

PORTNAME=	scanlnk
PORTVERSION=	0.1
PORTREVISION=	0
CATEGORIES=	sysutils

MAINTAINER=	awalker@ixsystems.com
COMMENT=	Quick check for symlinks pointing to device underlying "/"

USE_GITHUB=	yes
GH_ACCOUNT=	anodos325
GH_TAGNAME=	af27c550f3daa749cd1e099aa45bb37c9b75d6ec

PLIST_FILES=	bin/scanlnk 

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/scanlnk ${STAGEDIR}${PREFIX}/bin/scanlnk

.include <bsd.port.mk>
