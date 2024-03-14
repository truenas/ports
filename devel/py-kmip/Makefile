# $FreeBSD$

PORTNAME=	kmip
PORTVERSION=	0.10.0
CATEGORIES=	devel python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	waqar@ixsystems.com
COMMENT=	Python implementation of KMIP protocol

LICENSE=	APACHE20
LICENSE_FILE=	${WRKSRC}/LICENSE.txt

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}requests>=2.20:www/py-requests@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}cryptography-legacy>0:security/py-cryptography-legacy@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}sqlalchemy14>0:databases/py-sqlalchemy14@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}six>=1.8.0:devel/py-six@${PY_FLAVOR}

USE_GITHUB=	yes
GH_ACCOUNT=	OpenKMIP
GH_PROJECT=	PyKMIP
GH_TAGNAME=	53fa326bd43e8da1e7cc910c3188756184ee0758

USES=		python:3.7+
USE_PYTHON=	autoplist concurrent distutils
NO_ARCH=	yes

.include <bsd.port.mk>
