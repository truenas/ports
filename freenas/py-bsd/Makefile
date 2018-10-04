# $FreeBSD$

PORTNAME=	bsd
PORTVERSION=	20170330012459
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
GH_TAGNAME=	6d148fa1558dc6618397edb7578e08330e6011a5

USES=		python
USE_PYTHON=	autoplist cython distutils

.include <bsd.port.mk>
