# $FreeBSD$

PORTNAME=	bsd
PORTVERSION=	20190312143903
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
GH_TAGNAME=	5e304367471fd03792d41e52c200b66f82d21883

USES=		python
USE_PYTHON=	autoplist cython distutils

.include <bsd.port.mk>
