# $FreeBSD$

PORTNAME=	freenas-pkgtools
PORTVERSION=	20180926134750
PORTREVISION=	${REVISION}
CATEGORIES=	freenas
VALID_CATEGORIES+=	freenas

MAINTAINER=	dev@ixsystems.com
COMMENT=	FreeNAS sef\'s pkg tools

PRODUCT?=

USES=		python

.if exists(/usr/local_source/${PORTNAME})
PORTVERSION!=	git -C /usr/local_source/${PORTNAME} log -1 --pretty=format:%ct
EXTRACT_ONLY=
WRKSRC=		/usr/local_source/${PORTNAME}
.else
USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	freenas-pkgtools
GH_TAGNAME=	6b650fc4bd2c23da9dbea93941259b5e4b4103a1
.endif


.if exists(/usr/local_source/${PORTNAME})
# no checksum, we just used git!
checksum fetch:
	${ECHO_CMD} ${.TARGET} not needed because building direct
.endif

ALL_TARGET=obj all

MAKE_JOBS_UNSAFE=yes

post-install:
	${PYTHON_CMD} -m compileall ${STAGEDIR}${PREFIX}/lib/freenasOS
	(cd ${STAGEDIR}${PREFIX}/lib/freenasOS; ${FIND} . -type f \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print "lib/freenasOS/"$$2 }' >> ${TMPPLIST})

.include <bsd.port.mk>
