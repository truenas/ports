# Created by: Vladimir Vinogradenko <themylogin@gmail.com>
# $FreeBSD$

PORTNAME=	netsnmpagent
PORTVERSION=	0.6.1
CATEGORIES=	devel python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Python module that facilitates writing Net-SNMP subagents in Python

USE_GITHUB=	yes
GH_ACCOUNT=	pief
GH_PROJECT=	python-netsnmpagent
GH_TAGNAME=	572fc108a385d4f8e33989d34fb2aad60353c3e0

USES=		python
USE_PYTHON=	distutils autoplist

pre-configure:
	sed 's/@NETSNMPAGENT_VERSION@/0.6.1/' ${WRKSRC}/setup.py.in >  ${WRKSRC}/setup.py

.include <bsd.port.options.mk>

.include <bsd.port.mk>
