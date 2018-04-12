#
# $FreeBSD$
# $Id$
#

PORTNAME=	libzfs
PORTVERSION=	1.0.2017
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

CATEGORIES=	devel

MAINTAINER=     dev@ixsystems.com
COMMENT=        Python libzfs bindings

USES?=		python
USE_PYTHON=	autoplist cython distutils
EXTRACT_ONLY=

WRKSRC?=/usr/nas_source/py-libzfs

MAKE_ENV=       FREEBSD_SRC=/usr/src TRUEOS=yes EXPERIMENTAL=yes

.include <bsd.port.pre.mk>

checksum fetch:
	echo ${.TARGET} not needed because building direct

.include <bsd.port.post.mk>
