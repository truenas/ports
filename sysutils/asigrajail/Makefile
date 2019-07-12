# $FreeBSD$

PORTNAME=	asigrajail
PORTVERSION=	1
CATEGORIES=	sysutils
MASTER_SITES=	https://asigra-f611.kxcdn.com/jail/
DISTNAME=	asigra_migration_image_9b5802df_2019-07-19
EXTRACT_SUFX=	.tar.xz

EXTRACT_ONLY=

MAINTAINER=	waqar@ixsystems.com
COMMENT=	Award cloud backup and restore server application's plugin image

NO_BUILD=	yes

.include <bsd.port.pre.mk>

do-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/share/asigra
	${CP} ${DISTDIR}/${DISTNAME}${EXTRACT_SUFX} ${STAGEDIR}${PREFIX}/share/asigra/
	@${ECHO_CMD} "@dir share/asigra" >> ${TMPPLIST}
	${FIND} -s ${STAGEDIR}${PREFIX}/share/asigra -not -type d | ${SORT} | \
		${SED} -e 's#^${STAGEDIR}${PREFIX}/##' >> ${TMPPLIST}

.include <bsd.port.post.mk>
