PORTNAME=	netif
PORTVERSION=	1.0.20240503
CATEGORIES=	net python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	dev@ixsystems.com
COMMENT=	Python network configuration library for FreeBSD
WWW=		https://github.com/truenas/py-netif

LICENSE=	BSD2CLAUSE

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}bsd>0:devel/py-bsd@${PY_FLAVOR}

USES=		python
USE_GITHUB=	yes
GH_ACCOUNT=	truenas
GH_PROJECT=	py-netif
GH_TAGNAME=	9298aa9
USE_PYTHON=	autoplist cython distutils

HAS_CONFIGURE=	yes

post-install:
	@${STRIP_CMD} ${STAGEDIR}${PYTHONPREFIX_SITELIBDIR}/netif*.so

.include <bsd.port.mk>
