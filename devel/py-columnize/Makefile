# $FreeBSD$

PORTNAME=	columnize
PORTVERSION=	0.3.8
CATEGORIES=	devel python
MASTER_SITES=	CHEESESHOP
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	dev@ixsystems.com
COMMENT=	Format a simple (i.e. not nested) list into aligned columns.

BUILD_DEPENDS=	${PYTHON_PKGNAMEPREFIX}nose>=1.0:${PORTSDIR}/devel/py-nose
BUILD_DEPENDS+=	${PYTHON_PKGNAMEPREFIX}backports.shutil_get_terminal_size=1.0:${PORTSDIR}/devel/py-backports.shutil_get_terminal_size
RUN_DEPENDS=	${BUILD_DEPENDS}

USES=		python
USE_PYTHON=	autoplist distutils

.include <bsd.port.mk>
