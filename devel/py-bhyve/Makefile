# $FreeBSD$


PORTNAME=	bhyve
PORTVERSION=	1.0.2018
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

CATEGORIES=	devel

MAINTAINER=     dev@ixsystems.com
COMMENT=        Python libvmmapi bindings

USES?=		python:3.4+
USE_PYTHON=	autoplist cython distutils

USE_GITHUB=     yes
GH_ACCOUNT=	freenas
GH_PROJECT=	py-bhyve
GH_TAGNAME=	1b6199c

MAKE_ENV=       FREEBSD_SRC=/usr/src

.include <bsd.port.mk>
