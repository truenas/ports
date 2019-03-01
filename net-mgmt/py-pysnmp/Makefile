# Created by: Martin Jackson <mhjacks@swbell.net>
# $FreeBSD$

PORTNAME=	pysnmp
PORTVERSION=	4.4.9
CATEGORIES=	net-mgmt python
MASTER_SITES=	CHEESESHOP \
		SF/pysnmp/pysnmp/${PORTVERSION}
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	mhjacks@swbell.net
COMMENT=	SNMP framework for Python

LICENSE=	BSD2CLAUSE

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}pyasn1>=0.1.2:devel/py-pyasn1@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}pycrypto>=2.4.1:security/py-pycrypto@${PY_FLAVOR}

USES=		python
USE_PYTHON=	distutils autoplist

OPTIONS_DEFINE= DOCS EXAMPLES

NO_ARCH=	yes

EXAMPLESDIR=	${PREFIX}/share/examples/${PYTHON_PKGNAMEPREFIX}${PORTNAME}
DOCSDIR=	${PREFIX}/share/doc/${PYTHON_PKGNAMEPREFIX}${PORTNAME}

PORTDOCS=	*
PORTEXAMPLES=	*

BASEDOCS=	CHANGES.txt README.md TODO.txt THANKS.txt

post-install-DOCS-on:
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
	${INSTALL_DATA} ${BASEDOCS:S,^,${WRKSRC}/,} ${STAGEDIR}${DOCSDIR}
	(cd ${WRKSRC}/docs && ${COPYTREE_SHARE} . ${STAGEDIR}${DOCSDIR})

post-install-EXAMPLES-on:
	@${MKDIR} ${STAGEDIR}${EXAMPLESDIR}

post-install:
	${MKDIR} ${STAGEDIR}${DOCSDIR}
	(cd ${WRKSRC}/docs/source/docs && \
	    ${COPYTREE_SHARE} . ${STAGEDIR}${DOCSDIR})
	${MKDIR} ${STAGEDIR}${EXAMPLESDIR}
	(cd ${WRKSRC}/examples && ${COPYTREE_SHARE} . ${STAGEDIR}${EXAMPLESDIR})

.include <bsd.port.mk>
