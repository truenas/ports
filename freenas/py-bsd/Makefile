# $FreeBSD$

PORTNAME=	bsd
PORTVERSION=	20190308180306
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
GH_TAGNAME=	c01a9240e8adbd895dd1a1f3dcd9d4fc3d92546a

USES=		python
USE_PYTHON=	autoplist cython distutils

.include <bsd.port.mk>
