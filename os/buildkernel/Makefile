# $FreeBSD$

.include "../Makefile.common"

PORTNAME=	buildkernel
PORTVERSION=	${OS_PORTVERSION}
CATEGORIES=	os

MAINTAINER=	kris@ixsystems.com
COMMENT=	Buildkernel portition of os packages

BUILD_DEPENDS=	/usr/src/Makefile:os/src

PREFIX=/
EXTRACT_ONLY=
WRKSRC=/usr/src
WITHOUT_FBSD10_FIX=	yes

PLIST_FILES+=	/usr/dist/kernel.txz

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

build:
	cd ${WRKSRC} && make -j ${_MAKE_JOBS_NUMBER} \
		MAKEOBJDIRPREFIX=${WRKDIR}/obj \
		buildkernel

do-install:
	${MKDIR} -p ${STAGEDIR}/kernel
	${MKDIR} -p ${STAGEDIR}/usr/dist
	cd ${WRKSRC} && make -j ${_MAKE_JOBS_NUMBER} \
		MAKEOBJDIRPREFIX=${WRKDIR}/obj \
	       	DESTDIR=${STAGEDIR}/kernel \
		installkernel
	tar cvJf ${STAGEDIR}/usr/dist/kernel.txz -C ${STAGEDIR}/kernel .
	chflags -R noschg ${STAGEDIR}/kernel
	${RM} -rf ${STAGEDIR}/kernel

.include <bsd.port.post.mk>
