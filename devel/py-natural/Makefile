# $FreeBSD$

PORTNAME=	natural
PORTVERSION=	0.1.7
CATEGORIES=	devel python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	jpaetzel@FreeBSD.org
COMMENT=	Convert data to their natural (human-readable) format

LICENSE=	MIT

USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_TAGNAME=	6188bad

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}six>0:${PORTSDIR}/devel/py-six

USES=		python
USE_PYTHON=	autoplist distutils

.include <bsd.port.pre.mk>

.if ${PYTHON_MAJOR_VER} == "3"
post-stage:
	${SED} -i '' 's,from . import locale,import locale,' ${STAGEDIR}/${PYTHONPREFIX_SITELIBDIR}/natural/language.py ${STAGEDIR}/${PYTHONPREFIX_SITELIBDIR}/natural/number.py ${STAGEDIR}/${PYTHONPREFIX_SITELIBDIR}/natural/size.py
.endif

.include <bsd.port.post.mk>
