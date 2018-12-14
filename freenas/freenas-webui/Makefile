# $FreeBSD$

PORTNAME=	freenas-webui
PORTVERSION=	20181210141757
PORTREVISION=	${REVISION}

CATEGORIES=		freenas
VALID_CATEGORIES+=	freenas

MAINTAINER=	dev@ixsystems.com
COMMENT=	Angular 5 Web User Interface for FreeNAS

FETCH_DEPENDS=	git>0:devel/git \
		python2>0:lang/python2
BUILD_DEPENDS=	rsync>0:net/rsync \
		python2>0:lang/python2 \
		npm>=0:www/npm \
		yarn>=0:www/yarn

NO_BUILD=	yes

.if exists(/usr/local_source/${PORTNAME})
TIMENOW=	%Y%m%d%H%M%S
PORTVERSION=	${TIMENOW:gmtime}
WRKSRC=		/usr/local_source/${PORTNAME}
EXTRACT_ONLY=
.else
USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	webui
GH_TAGNAME=	85d5b958012a7766e7c34327c38acc2fc4ba2b2e
.endif


.if exists(/usr/local_source/${PORTNAME})
checksum:
	${ECHO_CMD} ${.TARGET} not needed because building direct

extract:
	:
.endif

configure:
	:

build:
	(cd ${WRKSRC} && ${LOCALBASE}/bin/yarn install \
		&& ${LOCALBASE}/bin/yarn run build:prod:aot)

do-install:
	${MKDIR} ${WRKDIR} 2>/dev/null || true
	(cd ${WRKSRC} && ${LOCALBASE}/bin/yarn install && ${LOCALBASE}/bin/yarn run build:prod:aot)
	mkdir -p ${STAGEDIR}${PREFIX}/www/webui/
	rsync -avl --exclude '.git' --exclude 'nas_ports' --exclude 'etc' --exclude 'sbin' ${WRKSRC}/dist/ ${STAGEDIR}${PREFIX}/www/webui/
	(cd ${STAGEDIR}${PREFIX}/www/webui; ${FIND} . -type f \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| cut -d " " -f 2- \
		| ${AWK} '{print "www/webui/"$$0 }' >> ${TMPPLIST})

.include <bsd.port.mk>
