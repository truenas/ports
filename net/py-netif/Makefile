# Created by: William Grzybowski <wg@FreeBSD.org>
# $FreeBSD$

PORTNAME=	netif
PORTVERSION=	20181005080931
CATEGORIES=	net python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	wg@FreeBSD.org
COMMENT=	Python network configuration library for FreeBSD

LICENSE=	BSD2CLAUSE

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}bsd>0:devel/py-bsd@${PY_FLAVOR} \
		${PY_ENUM34}

USE_GITHUB=     yes
GH_ACCOUNT=	freenas
GH_PROJECT=	py-netif
GH_TAGNAME=	5a16f770ae3a94b5540b90f299ff254401c14520

HAS_CONFIGURE=	yes
USES=		python
USE_PYTHON=	autoplist distutils cython

.include <bsd.port.mk>
