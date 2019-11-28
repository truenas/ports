# Created by: rene@FreeBSD.org
# $FreeBSD$

PORTNAME=	ws4py
PORTVERSION=	0.5.1
PORTREVISION=	2
CATEGORIES=	www python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	wg@FreeBSD.org
COMMENT=	WebSocket package for Python

LICENSE=	BSD3CLAUSE

USE_GITHUB=	yes
GH_ACCOUNT=	Lawouach
GH_PROJECT=	WebSocket-for-Python
GH_TAGNAME=	961c07ce16ce4eedc34ca1fdacd29442870feccc

USES=		python
USE_PYTHON=	distutils autoplist

NO_ARCH=	yes

OPTIONS_DEFINE=	CHERRYPY GEVENT TORNADO
OPTIONS_DEFAULT=CHERRYPY GEVENT
CHERRYPY_DESC=	CherryPy server support
GEVENT_DESC=	gevent-based client/server support
TORNADO_DESC=	Tornado client support

CHERRYPY_RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}cherrypy>=5.5.0:www/py-cherrypy@${PY_FLAVOR}
GEVENT_RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}gevent>=1.1.1:devel/py-gevent@${PY_FLAVOR}
TORNADO_RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}tornado>=4.2.3:www/py-tornado@${PY_FLAVOR}

# Do not install files which will not work
post-patch-CHERRYPY-off:
.for f in test/test_cherrypy.py ws4py/server/cherrypyserver.py
	@${RM} ${WRKSRC}/${f}
.endfor

post-patch-GEVENT-off:
.for f in ws4py/client/geventclient.py ws4py/server/geventserver.py
	@${RM} ${WRKSRC}/${f}
.endfor

post-patch-TORNADO-off:
.for f in ws4py/client/tornadoclient.py
	@${RM} ${WRKSRC}/${f}
.endfor

.include <bsd.port.mk>
