# Created by: William Grzybowski <wg@FreeBSD.org>

PORTNAME=	netif
PORTVERSION=	1.0.20200414
CATEGORIES=	net python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	wg@FreeBSD.org
COMMENT=	Python network configuration library for FreeBSD

LICENSE=	BSD2CLAUSE

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}bsd>0:devel/py-bsd@${PY_FLAVOR}

USE_GITHUB=     yes
GH_ACCOUNT=	freenas
GH_PROJECT=	py-netif
GH_TAGNAME=	3903f2c5de49a4d6c25ad23b14f1fdf114995443

HAS_CONFIGURE=	yes
USES=		python:3.6+
USE_PYTHON=	autoplist distutils cython
GNU_CONFIGURE=	yes

.include <bsd.port.mk>
