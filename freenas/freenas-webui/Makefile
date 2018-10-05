# $FreeBSD$

PORTNAME=	freenas-webui
PORTVERSION=	20181004190218
PORTREVISION=	${REVISION}

CATEGORIES=		freenas
VALID_CATEGORIES+=	freenas

MAINTAINER=	dev@ixsystems.com
COMMENT=	Angular 5 Web User Interface for FreeNAS

FETCH_DEPENDS=	npm:freenas/npm5 \
		git>0:devel/git \
		python2>0:lang/python2
BUILD_DEPENDS=	rsync>0:net/rsync \
		npm:freenas/npm5 \
		python2>0:lang/python2

NO_BUILD=	yes

.if exists(/usr/local_source/${PORTNAME})
PORTVERSION!=	git -C /usr/local_source/${PORTNAME} log -1 --pretty=format:%ct
WRKSRC=		/usr/local_source/${PORTNAME}
EXTRACT_ONLY=
.else
USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	webui
GH_TAGNAME=	a6ee4fa1333ee775bdef3dac7ce03793b7406947
.endif


.if exists(/usr/local_source/${PORTNAME})
checksum:
	${ECHO_CMD} ${.TARGET} not needed because building direct

extract:
	:
.endif

patch:
	${MKDIR} ${WRKDIR} 2>/dev/null || true
	(cd ${WRKSRC} && ${LOCALBASE}/bin/npm install && ${LOCALBASE}/bin/npm run build:prod:aot)

do-install:
	mkdir -p ${STAGEDIR}${PREFIX}/www/webui/
	rsync -avl --exclude '.git' --exclude 'nas_ports' --exclude 'etc' --exclude 'sbin' ${WRKSRC}/dist/ ${STAGEDIR}${PREFIX}/www/webui/
	(cd ${STAGEDIR}${PREFIX}/www/webui; ${FIND} . -type f \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| cut -d " " -f 2- \
		| ${AWK} '{print "www/webui/"$$0 }' >> ${TMPPLIST})

.include <bsd.port.mk>
