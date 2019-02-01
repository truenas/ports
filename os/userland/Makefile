# $FreeBSD$

PORTNAME=	userland
PORTVERSION=	20190012500
CATEGORIES=	os

MAINTAINER=	kris@ixsystems.com
COMMENT=	Port for the OS userland binaries

BUILD_DEPENDS=  src>=0:os/src

PREFIX=/
EXTRACT_ONLY=
WRKSRC=/usr/src
WITHOUT_FBSD10_FIX=	yes

.if !exists(${WRKSRC}/Makefile)
IGNORE=	requires source files in ${WRKSRC}
.endif

.include "Makefile.options"
.include "Makefile.options.desc"

.include <bsd.port.pre.mk>

.if !${PORT_OPTIONS:MZFS}
MFLAGS+=	WITHOUT_ZFS=YES
.else
MFLAGS+=	WITH_ZFS=YES
.endif


.if exists(/usr/local_source/${PORTNAME})
checksum fetch:
	${ECHO_MGS} ${.TARGET} not needed because building direct
.endif

MFLAGS+=	MAKEOBJDIRPREFIX=${WRKDIR}/obj


build:
	cd ${WRKSRC} && env -i make -j ${_MAKE_JOBS_NUMBER} ${MFLAGS} buildworld

do-install:
	${MKDIR} -p ${STAGEDIR}
	cd ${WRKSRC} && env -i make -j ${_MAKE_JOBS_NUMBER} \
		${MFLAGS} \
	       	DESTDIR=${STAGEDIR} \
		installworld
	cd ${WRKSRC} && env -i make -j ${_MAKE_JOBS_NUMBER} \
		${MFLAGS} \
		DESTDIR=${STAGEDIR} \
		distribution
	(cd ${STAGEDIR}; ${FIND} . \( -type f -o -type l \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' >> ${TMPPLIST})
	(cd ${STAGEDIR}; ${FIND} ./ \( -type d \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' \
		| ${GREP} -v -e '/' \
		| ${XARGS} -I '{}' echo "@dir {}" >> ${TMPPLIST})

clean:
	@if [ -d ${WRKDIR} ]; then \
		if [ -w ${WRKDIR} ]; then \
			${ECHO_MSG} "===> Cleaning for ${PKGNAME}"; \
			chflags -R noschg ${WRKDIR} ; \
			${RM} -r ${WRKDIR}; \
		else \
			${ECHO_MSG} "===>   ${WRKDIR} not writable, skipping"; \
		fi; \
	fi

.include <bsd.port.post.mk>
