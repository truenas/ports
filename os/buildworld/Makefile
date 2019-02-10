# $FreeBSD$

.include "../Makefile.common"

PORTNAME=	buildworld
PORTVERSION=	${OS_PORTVERSION}
CATEGORIES=	os

MAINTAINER=	kris@ixsystems.com
COMMENT=	Port for the buildworld phase of OS

SRCDIR?=	/usr/src
BUILD_DEPENDS=	${SRCDIR}/Makefile:os/src

PREFIX=/
EXTRACT_ONLY=
DISTFILES=
SRCDIR?=/usr/src
WRKSRC=${SRCDIR}
WITHOUT_FBSD10_FIX=	yes
PKG_ABISTRING=	FreeBSD:13:${ARCH}
LOCAL_SDIR?=	/usr/local_source
.if exists(${LOCAL_SDIR}/${PORTNAME})
WRKSRC=	${LOCAL_SDIR}/${PORTNAME}
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

.if defined(_MAKE_JOBS_NUMBER)
JOBFLAG=	-j ${_MAKE_JOBS_NUMBER}
.endif

PLIST_FILES+=	usr/dist/world.txz

# This is broken right now
#MFLAGS+=	MAKEOBJDIRPREFIX=${WRKDIR}/obj


do-build:
	cd ${WRKSRC} && env -i make ${JOBFLAG} ${WORLD_MAKE_FLAGS} \
		${MFLAGS} \
		buildworld

do-install:
	${MKDIR} -p ${STAGEDIR}/world
	${MKDIR} -p ${STAGEDIR}/usr/dist
	cd ${WRKSRC} && env -i make ${JOBFLAG} ${WORLD_MAKE_FLAGS} \
		${MFLAGS} \
	       	DESTDIR=${STAGEDIR}/world \
		installworld
	cd ${WRKSRC} && env -i make ${JOBFLAG} ${WORLD_MAKE_FLAGS} \
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
