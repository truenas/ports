#
# $FreeBSD$
# $Id$
#

PORTNAME=      	libzfs
PORTVERSION=	1.0
PORTREVISION=	2
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

CATEGORIES=	devel

MAINTAINER=     dev@ixsystems.com
COMMENT=        Python libzfs bindings

USES?=		python
USE_PYTHON=	autoplist distutils
EXTRACT_ONLY=

WRKSRC?=/usr/nas_source/py-libzfs

MAKE_ENV=       FREEBSD_SRC=/usr/src TRUEOS=yes EXPERIMENTAL=yes

.include <bsd.port.pre.mk>

BUILD_DEPENDS=  cython>0:lang/cython

.if ${PYTHON_REL} < 3400
RUN_DEPENDS+=   ${PYTHON_PKGNAMEPREFIX}enum34>0:devel/py-enum34@${FLAVOR}
.endif

checksum fetch:
	echo ${.TARGET} not needed because building direct

.include <bsd.port.post.mk>
