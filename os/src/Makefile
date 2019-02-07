# $FreeBSD$

PORTNAME=	src
PORTVERSION=	20190012520
CATEGORIES=	os

MAINTAINER=	kris@ixsystems.com
COMMENT=	Port for the OS base sources

USE_GITHUB=	yes
GH_ACCOUNT=	trueos
GH_PROJECT=	trueos
GH_TAGNAME=	afb9da498a1e6bfb3908a15c1efbba873422f80d

PREFIX=/
SRCDIR?=/usr/src
NO_BUILD=	yes
WITHOUT_FBSD10_FIX=	yes
PKG_ABISTRING=	FreeBSD:13:${ARCH}

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
	(cd ${STAGEDIR}; ${FIND} .${SRCDIR} \( -type d \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' \
		| ${XARGS} -I '{}' echo "@dir {}" >> ${TMPPLIST})

.include <bsd.port.post.mk>
