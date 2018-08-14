# Created by: John Hixson (jhixson@FreeBSD.org)
# $FreeBSD$

PORTNAME=	dsoperator
PORTVERSION=	14.0.0.1
CATEGORIES=	sysutils
MASTER_SITES=	http://www.asigra.com/updates/14.0/DS-Operator/14.0.0.1/
DISTNAME=	DS-Operator_FreeBSD_14_0_0_1

MAINTAINER=	jhixson@FreeBSD.org
COMMENT=	Award cloud backup and restore server application

ONLY_FOR_ARCHS=	amd64

NO_WRKSUBDIR=	yes

NO_BUILD=	yes
USES=		zip

do-install:
	${MKDIR} ${STAGEDIR}${LOCALBASE}/www/asigra
	(cd ${WRKSRC} && ${COPYTREE_BIN} . ${STAGEDIR}${LOCALBASE}/www/asigra)

.include <bsd.port.mk>
