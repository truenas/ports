# $FreeBSD$

PORTNAME=	bsd
PORTVERSION=	20190306125847
PORTREVISION=	0
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

CATEGORIES=	freenas
VALID_CATEGORIES+= freenas

MAINTAINER=	dev@ixsystems.com
COMMENT=	Python wrappers for various BSD interfaces

BUILD_DEPENDS=  ${PYTHON_PKGNAMEPREFIX}six>0:devel/py-six@${FLAVOR}

USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	py-bsd
GH_TAGNAME=	e543f5cadbd185d65dbea4a4d574f53ff7b9351f

USES=		python
USE_PYTHON=	autoplist cython distutils

.include <bsd.port.mk>
