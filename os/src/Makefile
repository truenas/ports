# $FreeBSD$

PORTNAME=	src
PORTVERSION=	20190012520
CATEGORIES=	os

MAINTAINER=	kris@ixsystems.com
COMMENT=	Port for the OS base sources

.if exists(/usr/local_source/${PORTNAME})
PORTVERSION!=	stat -f %m /usr/local_source/${PORTNAME}
EXTRACT_ONLY=
WRKSRC=         /usr/local_source/${PORTNAME}
.else
USE_GITHUB=	yes
GH_ACCOUNT=	trueos
GH_PROJECT=	trueos
GH_TAGNAME=	afb9da498a1e6bfb3908a15c1efbba873422f80d
.endif

PREFIX=/
SRCDIR=/usr/src
NO_BUILD=	yes
WITHOUT_FBSD10_FIX=	yes

.include <bsd.port.pre.mk>

.if exists(/usr/local_source/${PORTNAME})
checksum fetch:
	${ECHO_MGS} ${.TARGET} not needed because building direct
.endif

do-install:
	${MKDIR} -p ${STAGEDIR}${SRCDIR}
	${TAR} cf - -C ${WRKSRC} --exclude .git --exclude .svn . | ${TAR} xpf - -C ${STAGEDIR}${SRCDIR}
	(cd ${STAGEDIR}; ${FIND} . \( -type f -o -type l \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' >> ${TMPPLIST})
	(cd ${STAGEDIR}; ${FIND} ./usr/src \( -type d \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' \
		| ${XARGS} -I '{}' echo "@dir {}" >> ${TMPPLIST})

.include <bsd.port.post.mk>
