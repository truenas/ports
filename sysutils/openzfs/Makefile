# $FreeBSD$

PORTNAME=	openzfs
PORTVERSION=	2022010600
DISTVERSIONPREFIX=	v
CATEGORIES=	sysutils

MAINTAINER=	ryan@iXsystems.com
COMMENT=	OpenZFS userland for FreeBSD

LICENSE=	CDDL

BROKEN_aarch64=	invokes x86 assembler

RUN_DEPENDS=	openzfs-kmod>=0:sysutils/openzfs-kmod

USES+=		autoreconf:build libtool gmake gettext-runtime:run python:3.3+,build shebangfix

GNU_CONFIGURE=  yes
CONFIGURE_ARGS=	--prefix=${PREFIX} --with-config="user"

OPTIONS_DEFINE= DEBUG PYTHON TESTS
OPTIONS_SUB=    yes

USE_GITHUB=	yes
GH_ACCOUNT=	truenas
GH_PROJECT=	zfs
GH_TAGNAME=	9d6ba573d

.include <bsd.port.pre.mk>

.if (${OSVERSION} < 1200504) || (${OSVERSION} >= 1300000 && ${OSVERSION} < 1300013)
IGNORE=      needs FreeBSD 12/13 with AES-CCM support
.endif

.if ${PORT_OPTIONS:MDEBUG}
CONFIGURE_ARGS+=	--enable-debug --enable-debuginfo
.endif

.if ${PORT_OPTIONS:MPYTHON}
CONFIGURE_ARGS+=	--with-python=${PYTHON_VER}
BUILD_DEPENDS+=	${PYTHON_PKGNAMEPREFIX}packaging>0:devel/py-packaging@${PY_FLAVOR}
BUILD_DEPENDS+=	${PYTHON_PKGNAMEPREFIX}cffi>0:devel/py-cffi@${PY_FLAVOR}
RUN_DEPENDS+=	${PYTHON_PKGNAMEPREFIX}cffi>0:devel/py-cffi@${PY_FLAVOR}
RUN_DEPENDS+=	${PYTHON_PKGNAMEPREFIX}sysctl>0:devel/py-sysctl@${PY_FLAVOR}
.else
CONFIGURE_ARGS+=	--without-python
.endif

pre-configure:
	cd ${WRKSRC} && ./autogen.sh

post-stage:
	${RM} -rf ${STAGEDIR}/usr/lib

.include <bsd.port.post.mk>
