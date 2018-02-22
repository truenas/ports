PORTNAME=	asyncssh
PORTVERSION=	1.12.0
CATEGORIES=	security python
MASTER_SITES=	CHEESESHOP
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=  	bartosz@ixsystems.com
COMMENT=	Python asnycio SSH protocol library

LICENSE=	EPL
LICENSE_FILE=	${WRKSRC}/LICENSE

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}bcrypt>0:security/py-bcrypt \
		${PYTHON_PKGNAMEPREFIX}cryptography>0:security/py-cryptography

USES?=		python:3
USE_PYTHON=	autoplist concurrent distutils

.include <bsd.port.mk>
