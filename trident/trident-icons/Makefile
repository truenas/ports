# $FreeBSD$

PORTNAME=	trident-icons
PORTVERSION=	201805211346
CATEGORIES=	trident

MAINTAINER=	jt@ixsystems.com
COMMENT=	Icon Themes for Project Trident

LICENSE=	BSD3CLAUSE

USE_GITHUB=	yes
GH_ACCOUNT=	project-trident
GH_PROJECT=	trident-icons
GH_TAGNAME=	1dfefa0d6208a51b82b07d163a39458d631cd884

NO_BUILD=	yes

do-install:
	${MKDIR} ${STAGEDIR}/share/icons
	cd ${WRKSRC} && ${WRKSRC}/install-local.sh ${STAGEDIR}/usr/local/share/icons

.include <bsd.port.mk>
