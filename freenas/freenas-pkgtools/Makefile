# $FreeBSD$

PORTNAME=	freenas-pkgtools
PORTVERSION=	20190227132243
PORTREVISION=	${REVISION}
CATEGORIES=	freenas
VALID_CATEGORIES+=	freenas

MAINTAINER=	dev@ixsystems.com
COMMENT=	FreeNAS sef\'s pkg tools

PRODUCT?=

USES=		python

.if exists(/usr/ports/local_source/${PORTNAME})
PORTVERSION!=	stat -f %m /usr/ports/local_source/${PORTNAME}
EXTRACT_ONLY=
WRKSRC=		/usr/ports/local_source/${PORTNAME}
.else
USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	freenas-pkgtools
GH_TAGNAME=	7f6072f41a25ef95f2abeebaef5429cb19481258
.endif


.if exists(/usr/ports/local_source/${PORTNAME})
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
