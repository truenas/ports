# New ports collection makefile for: gnome settings in live mode
# Date created:        11 may 2015
# Whom:                angelescuo@yahoo.com
#
# $FreeBSD$
#

PORTNAME=	ghostbsd-slim-theme
PORTVERSION=	1.3
CATEGORIES=	x11

MAINTAINER=	ericbsd@FreeBSD.org
COMMENT=	GhostBSD theme for slim and autologin too

LICENSE=	BSD

USE_GITHUB=	yes
GH_ACCOUNT=	GhostBSD

RUN_DEPENDS=	slim:x11/slim \

NO_BUILD=	yes

do-install:
	${MKDIR} ${STAGEDIR}/${PREFIX}/share/slim/themes/ghostbsd
	${CP} -R ${WRKSRC}/gbsd/* ${STAGEDIR}/${PREFIX}/share/slim/themes/ghostbsd/

.include <bsd.port.mk>
