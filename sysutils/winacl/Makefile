# $FreeBSD$

PORTNAME=	winacl
PORTVERSION=	0.1
PORTREVISION=	0
CATEGORIES=	sysutils

MAINTAINER=	awalker@ixsystems.com
COMMENT=	multi-purpose tool for working with NFSv4 ACLs

USE_GITHUB=	yes
GH_ACCOUNT=	anodos325
GH_TAGNAME=	4e67113a4b5184516c40fd63dcad0f7940fcb9e7

PLIST_FILES=	bin/winacl

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/winacl ${STAGEDIR}${PREFIX}/bin/winacl

.include <bsd.port.mk>
