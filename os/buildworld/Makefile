# $FreeBSD$

.include "../Makefile.common"

PORTNAME=	buildworld
PORTVERSION=	${OS_PORTVERSION}
CATEGORIES=	os

MAINTAINER=	kris@ixsystems.com
COMMENT=	Port for the buildworld phase of OS

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

.for var in ${OPTIONS_DEFINE}
.if !${PORT_OPTIONS:M${var}}
MFLAGS+=	WITHOUT_${var}=YES
.else
MFLAGS+=	WITH_${var}=YES
.endif
.endfor

PLIST_FILES+=	usr/dist/world.txz

# This is broken right now
#MFLAGS+=	MAKEOBJDIRPREFIX=${WRKDIR}/obj


build:
	cd ${WRKSRC} && env -i make -j ${_MAKE_JOBS_NUMBER} ${MFLAGS} buildworld

do-install:
	${MKDIR} -p ${STAGEDIR}/world
	${MKDIR} -p ${STAGEDIR}/usr/dist
	cd ${WRKSRC} && env -i make -j ${_MAKE_JOBS_NUMBER} \
		${MFLAGS} \
	       	DESTDIR=${STAGEDIR}/world \
		installworld
	cd ${WRKSRC} && env -i make -j ${_MAKE_JOBS_NUMBER} \
		${MFLAGS} \
		DESTDIR=${STAGEDIR}/world \
		distribution
	tar cvJf ${STAGEDIR}/usr/dist/world.txz -C ${STAGEDIR}/world .
	chflags -R noschg ${STAGEDIR}/world
	rm -rf ${STAGEDIR}/world

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
