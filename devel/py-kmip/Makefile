# $FreeBSD$

PORTNAME=	kmip
PORTVERSION=	0.9.1
CATEGORIES=	devel python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	waqar@ixsystems.com
COMMENT=	Python implementation of KMIP protocol

LICENSE=	APACHE20
LICENSE_FILE=	${WRKSRC}/LICENSE.txt

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}requests>=2.20:www/py-requests@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}cryptography>0:security/py-cryptography@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}sqlalchemy12>0:databases/py-sqlalchemy12@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}six>=1.8.0:devel/py-six@${PY_FLAVOR}

USE_GITHUB=	yes
GH_ACCOUNT=	OpenKMIP
GH_PROJECT=	PyKMIP
GH_TAGNAME=	ea6637239cb3dd1a36f8281dcac9fd0716d741d4

USES=		python:3.7+
USE_PYTHON=	autoplist concurrent distutils
NO_ARCH=	yes

.include <bsd.port.mk>
