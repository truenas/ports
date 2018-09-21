# $FreeBSD$


PORTNAME=	bhyve
PORTVERSION=	20180905
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

CATEGORIES=	devel python

MAINTAINER=	dev@ixsystems.com
COMMENT=	Python libvmmapi bindings

LICENSE=	BSD2CLAUSE

USES?=	python:3.4+
USE_PYTHON=	autoplist cython distutils

USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	py-bhyve
GH_TAGNAME=	1b6199c

MAKE_ENV=	FREEBSD_SRC=/usr/src

.include <bsd.port.mk>
