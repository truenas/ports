# $FreeBSD$

PORTNAME=	cam
PORTVERSION=	1.0.20190813
CATEGORIES=	devel python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	wg@FreeBSD.org
COMMENT=	Python wrapper for FreeBSD CAM

LICENSE=	BSD3CLAUSE

RUN_DEPENDS=	${PY_ENUM34}

USES=		python
USE_PYTHON=	autoplist distutils cython

USE_GITHUB=     yes
GH_ACCOUNT=	freenas
GH_PROJECT=	py-cam
GH_TAGNAME=	cc321aba2fc37dcb5c6d65600637ad23b2ecadd8

MAKE_ENV=       FREEBSD_SRC=${SRC_BASE}

.include <bsd.port.options.mk>

.if !exists(${SRC_BASE}/sys/Makefile)
IGNORE= requires kernel source files in ${SRC_BASE}
.endif

.include <bsd.port.mk>
