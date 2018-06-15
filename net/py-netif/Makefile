# Created by: William Grzybowski <wg@FreeBSD.org>
# $FreeBSD: head/net/py-netif/Makefile 430454 2017-01-03 11:01:05Z mat $

PORTNAME=	netif
PORTVERSION=	1.0.20180615
CATEGORIES=	net python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	wg@FreeBSD.org
COMMENT=	Python network configuration library for FreeBSD

LICENSE=	BSD2CLAUSE

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}bsd>0:devel/py-bsd@${FLAVOR} \
		${PY_ENUM34}

USE_GITHUB=     yes
GH_ACCOUNT=	freenas
GH_PROJECT=	py-netif
GH_TAGNAME=	1c657fb

USES=		python
USE_PYTHON=	autoplist distutils cython

.include <bsd.port.mk>
