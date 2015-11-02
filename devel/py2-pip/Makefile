# Created by: Nicola Vitale <nivit@FreeBSD.org>
# $FreeBSD$

PORTNAME=	pip
PORTVERSION=	7.0.3
CATEGORIES=	devel python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	nivit@FreeBSD.org
COMMENT=	Tool for installing and managing Python packages

LICENSE=	MIT

# Tests also need bzr, git, otherwise they fail

USES=		python:2 shebangfix
USE_GITHUB=	yes
USE_PYTHON=	autoplist concurrent distutils

SHEBANG_FILES=	pip/__init__.py
python_OLD_CMD=	/usr/bin/env python

GH_ACCOUNT=	pypa

.include <bsd.port.options.mk>

post-install:
#	${RM} ${STAGEDIR}${PREFIX}/bin/pip2

regression-test: build
	@cd ${WRKSRC} && ${PYTHON_CMD} ${PYDISTUTILS_SETUP} test

.include <bsd.port.mk>
