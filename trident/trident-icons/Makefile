# $FreeBSD$

PORTNAME=	trident-icons
PORTVERSION=	201806190820
CATEGORIES=	trident

MAINTAINER=	jt@ixsystems.com
COMMENT=	Icon Themes for Project Trident

LICENSE=	BSD3CLAUSE

USE_GITHUB=	yes
GH_ACCOUNT=	project-trident
GH_PROJECT=	trident-icons
GH_TAGNAME=	188e9867c56ebf7d75415a34ec7e98ebf348a66b

NO_BUILD=	yes

do-install:
	${MKDIR} ${STAGEDIR}/usr/local/share/icons
	cd ${WRKSRC} && ${WRKSRC}/install-local.sh ${STAGEDIR}/usr/local/share/icons

.include <bsd.port.mk>
