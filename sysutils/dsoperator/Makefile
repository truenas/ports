# $FreeBSD$

PORTNAME=	dsoperator
PORTVERSION=	14.1
CATEGORIES=	sysutils
MASTER_SITES=	http://builds.ixsystems.com/ix-iso/asigra/14.1/
DISTNAME=	DS-Operator

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
