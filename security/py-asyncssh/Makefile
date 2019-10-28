# $FreeBSD$

PORTNAME=	asyncssh
PORTVERSION=	2.0.0
CATEGORIES=	security python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	sunpoet@FreeBSD.org
COMMENT=	Python asnycio SSH protocol library

LICENSE=	EPL
LICENSE_FILE=	${WRKSRC}/LICENSE

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}bcrypt>0:security/py-bcrypt@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}cryptography>0:security/py-cryptography@${PY_FLAVOR}

USE_GITHUB=	yes
GH_ACCOUNT=	ronf
GH_PROJECT=	asyncssh
GH_TAGNAME=	bcd1c2bb3b5671e2a1d32965d937bd41acfa4cda

USES=		python:3.7+
USE_PYTHON=	autoplist concurrent distutils
NO_ARCH=	yes

do-test:
	@cd ${WRKSRC} && ${PYTHON_CMD} ${PYDISTUTILS_SETUP} test

.include <bsd.port.mk>
