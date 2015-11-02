# Created by: William Grzybowski <wg@FreeBSD.org>
# $FreeBSD$

PORTNAME=	cffi
PORTVERSION=	1.2.1
CATEGORIES=	devel python
MASTER_SITES=	CHEESESHOP
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	wg@FreeBSD.org
COMMENT=	Foreign Function Interface for Python calling C code

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/LICENSE

LIB_DEPENDS=	libffi.so:${PORTSDIR}/devel/libffi
RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}pycparser>=2.10:${PORTSDIR}/devel/py2-pycparser

CFLAGS+=	-I${LOCALBASE}/include -Wl,-rpath,${LOCALBASE}/lib
LDFLAGS+=	-L${LOCALBASE}/lib

USES=		python:2
USE_PYTHON=	autoplist distutils

regression-test: patch
	cd ${WRKSRC} && ${PYTHON_CMD} ${PYDISTUTILS_SETUP} build_ext -i &&
	${LOCALBASE}/bin/py.test

.include <bsd.port.mk>
