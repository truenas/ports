# $FreeBSD$

PORTNAME=	wsdd	
PORTVERSION=	1.0		
PORTREVISION=	0	
CATEGORIES=	sysutils python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	awalker@ixsystems.com
COMMENT=	Python WS-Discovery implementation	

USES=		python:3.6+
USE_PYTHON=	distutils
USE_GITHUB=	yes
GH_ACCOUNT=	freenas	
GH_PROJECT=	wsdd
GH_TAGNAME=	0195eff4972dd5ef65c050e312f5677cda0617ec	

post-install:
		${LN} -sf ${PYTHON_SITELIBDIR}/wsdd/wsdd.py ${LOCALBASE}/bin/wsdd.py
		chmod +x ${LOCALBASE}/bin/wsdd.py	

.include <bsd.port.mk>
