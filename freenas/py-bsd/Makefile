# $FreeBSD$

PORTNAME=	bsd
PORTVERSION=	20180925152452
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
GH_TAGNAME=	2544604bd34cf76ab65ac1d4ba07b9c29bffc848

USES=		python
USE_PYTHON=	autoplist cython distutils

.include <bsd.port.mk>
