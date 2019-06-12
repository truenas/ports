# $FreeBSD$

PORTNAME=	dsoperator
PORTVERSION=	14.1
PORTREVISION=	1
CATEGORIES=	sysutils
MASTER_SITES=	https://asigra-f611.kxcdn.com/14.1/hotfixes/
DISTNAME=	DS-Operator_FreeBSD_14_0_0_1
EXTRACT_SUFX=	.zip

MAINTAINER=	wg@FreeBSD.org
COMMENT=	Award cloud backup and restore server application

ONLY_FOR_ARCHS=	amd64

NO_WRKSUBDIR=	yes

NO_BUILD=	yes
USES=		zip

do-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/www/asigra
	(cd ${WRKSRC} && ${COPYTREE_BIN} . ${STAGEDIR}${PREFIX}/www/asigra)
	${CP} ${FILESDIR}/DSOP.jnlp.template ${STAGEDIR}${PREFIX}/www/asigra/DSOP.jnlp.template

.include <bsd.port.mk>
