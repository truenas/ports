# $FreeBSD$

PORTNAME=	webob
PORTVERSION=	1.4
CATEGORIES=	www python
MASTER_SITES=	CHEESESHOP
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}
DISTNAME=	WebOb-${DISTVERSION}

MAINTAINER=	nivit@FreeBSD.org
COMMENT=	WSGI request and response Python object

LICENSE=	MIT

DOCSDIR=	${PREFIX}/share/doc/${PYTHON_PKGNAMEPREFIX}${PORTNAME}

OPTIONS_DEFINE= DOCS TESTS

PORTDOCS=	*

USES=		python:2
USE_PYTHON=	distutils autoplist

TESTS_BUILD_DEPENDS=	nosetests:${PORTSDIR}/devel/py-nose
TESTS_DESC=	Run tests with Nose

.include <bsd.port.options.mk>

.if ${PORT_OPTIONS:MTESTS}
PYDISTUTILS_BUILD_TARGET+=	nosetests
.endif

.include <bsd.port.mk>
