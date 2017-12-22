# $FreeBSD$

PORTNAME=	py-libstoraj
PORTVERSION=	1.0.2
DISTVERSIONPREFIX=	v
CATEGORIES=	net	

MAINTAINER=	jhixson@gmail.com
COMMENT=	Python bindings for libstorj

LICENSE=	LGPL21+

BUILD_DEPENDS=	swig1.3:devel/swig13
LIB_DEPENDS=	libstorj.so:sysutils/libstorj

post-patch:
	@cd ${WRKSRC}; ${CP} -a ${WRKSRC_libstorj}/ ${WRKSRC}/ext/libstorj; \
	${LOCALBASE}/bin/swig1.3 -c++ -python -outdir lib/ext lib/ext/python_libstorj.i

post-install:
	${INSTALL_PROGRAM} ${WRKSRC}/lib/ext/_python_libstorj.so \
		${STAGEDIR}${PYTHONPREFIX_SITELIBDIR}/_python_libstorj.so

USE_GITHUB=	yes
GH_ACCOUNT=	Storj
GH_PROJECT=	python-libstorj
GH_TAGNAME=	b344699
GH_TUPLE=	Storj:libstorj:v1.0.2:libstorj

USES=	python:2.7+
USE_PYTHON=	distutils autoplist

CFLAGS+=	-I. -I${LOCALBASE}/include
LDFLAGS+=	-L. -L${LOCALBASE}/lib

.include <bsd.port.mk>
