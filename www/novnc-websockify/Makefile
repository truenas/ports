# $FreeBSD$

PORTNAME=	websockify
PORTVERSION=	0.8.0
PORTREVISION=	2
CATEGORIES=	www

MAINTAINER=	araujo@FreeBSD.org
COMMENT=	noVNC-websockify implementation.

RUN_DEPENDS=	novnc>0:www/novnc

USES=		shebangfix python
SHEBANG_FILES=	run

USE_GITHUB=	yes
GH_ACCOUNT=	novnc
GH_PROJECT=	websockify
GH_TAGNAME=	eca301c05b5a5ae9b6896128a44fe2b6c7883c4d

NO_BUILD=	yes
NO_ARCH=	yes
TARGET_DIR=	${PREFIX}/libexec/novnc/utils/websockify

do-install:
	@${MKDIR} ${STAGEDIR}${TARGET_DIR}
	cd ${WRKSRC} && ${RM} .gitignore .gitmodules .travis.yml
	cd ${WRKSRC} && ${COPYTREE_SHARE} . ${STAGEDIR}${TARGET_DIR}
	@${CHMOD} +x ${STAGEDIR}${TARGET_DIR}/run
	@${CHMOD} +x ${STAGEDIR}${TARGET_DIR}/rebind

.include <bsd.port.mk>
