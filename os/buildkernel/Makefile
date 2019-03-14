# $FreeBSD$

.include "../Makefile.common"

PORTNAME=	buildkernel
PORTVERSION=	${OS_PORTVERSION}
CATEGORIES=	os

MAINTAINER=	kris@ixsystems.com
COMMENT=	Buildkernel portition of os packages

SRCDIR?=/usr/src
BUILD_DEPENDS=	${SRCDIR}/Makefile:os/src

PREFIX=/
EXTRACT_ONLY=
DISTFILES=
WITHOUT_FBSD10_FIX=	yes
WRKSRC=${SRCDIR}
PLIST_FILES+=	/usr/dist/kernel.txz
PKG_ABISTRING=	FreeBSD:13:${ARCH}
LOCAL_SDIR?=	/usr/local_source
.if exists(${LOCAL_SDIR}/${PORTNAME})
WRKSRC=	${LOCAL_SDIR}/${PORTNAME}
.endif

.if defined(_MAKE_JOBS_NUMBER)
JOBFLAG=	-j ${_MAKE_JOBS_NUMBER}
.else
JOBFLAG=	-j ${_SMP_CPUS}
.endif

.if defined(__MAKE_CONF)
MCONF=	__MAKE_CONF=${__MAKE_CONF}
.else
MCONF=
.endif

.include "../buildworld/Makefile.options"
.include "../buildworld/Makefile.options.desc"

.include <bsd.port.pre.mk>

.for var in ${OPTIONS_DEFINE}
.if !${PORT_OPTIONS:M${var}}
MFLAGS+=	WITHOUT_${var}=YES
.else
MFLAGS+=	WITH_${var}=YES
.endif
.endfor

do-build:
	cd ${WRKSRC} && env -i make ${JOBFLAG} ${MCONF} ${KERNEL_MAKE_FLAGS} \
		${MFLAGS} \
		buildkernel

do-install:
	${MKDIR} -p ${STAGEDIR}/kernel
	${MKDIR} -p ${STAGEDIR}/usr/dist
	cd ${WRKSRC} && env -i make ${JOBFLAG} ${MCONF} ${KERNEL_MAKE_FLAGS} \
		${MFLAGS} \
	       	DESTDIR=${STAGEDIR}/kernel \
		installkernel
	tar cvJf ${STAGEDIR}/usr/dist/kernel.txz -C ${STAGEDIR}/kernel .
	chflags -R noschg ${STAGEDIR}/kernel
	${RM} -rf ${STAGEDIR}/kernel

.include <bsd.port.post.mk>
