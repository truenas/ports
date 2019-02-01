# $FreeBSD$

PORTNAME=	kernel-generic
PORTVERSION=	20190012500
CATEGORIES=	os

MAINTAINER=	kris@ixsystems.com
COMMENT=	Port for the OS GENERIC kernel

BUILD_DEPENDS=	/usr/src/Makefile:os/src

PREFIX=/
EXTRACT_ONLY=
WRKSRC=/usr/src
WITHOUT_FBSD10_FIX=	yes

.if !exists(${WRKSRC}/Makefile)
IGNORE=	requires source files in ${WRKSRC}
.endif

.include <bsd.port.pre.mk>

.if exists(/usr/local_source/${PORTNAME})
checksum fetch:
	${ECHO_MGS} ${.TARGET} not needed because building direct
.endif

build:
	cd ${WRKSRC} && make -j ${_MAKE_JOBS_NUMBER} \
		MAKEOBJDIRPREFIX=${WRKDIR}/obj \
		buildkernel

do-install:
	${MKDIR} -p ${STAGEDIR}
	cd ${WRKSRC} && make -j ${_MAKE_JOBS_NUMBER} \
		MAKEOBJDIRPREFIX=${WRKDIR}/obj \
	       	DESTDIR=${STAGEDIR} \
		installkernel
	(cd ${STAGEDIR}; ${FIND} . \( -type f -o -type l \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' >> ${TMPPLIST})
	(cd ${STAGEDIR}; ${FIND} ./boot/kernel \( -type d \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' \
		| ${XARGS} -I '{}' echo "@dir {}" >> ${TMPPLIST})

.include <bsd.port.post.mk>
