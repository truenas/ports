# $FreeBSD$

PORTNAME=       freenas-docs
PORTVERSION=	20180828150723
PORTREVISION=	${REVISION}

CATEGORIES=     freenas
VALID_CATEGORIES+=	freenas

MAINTAINER=     dev@ixsystems.com
COMMENT=        Userguide for UI
PRODUCT?=

BUILD_DEPENDS=	sphinx-build:textproc/py-sphinx@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}sphinxcontrib-httpdomain>0:textproc/py-sphinxcontrib-httpdomain@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}sphinx_numfig>0:textproc/py-sphinx_numfig@${PY_FLAVOR}

USES=	perl5 python:build

USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	freenas-docs
GH_TAGNAME=	e5c140e9117df1a6832d5c8fb9571d06e4fc3406

.include <bsd.port.pre.mk>

.if ${PRODUCT} == "TrueNAS"
USES+=		gmake
BUILD_DEPENDS+=	gmake>0:devel/gmake \
		tex-xetex>0:print/tex-xetex \
		tex-dvipdfmx>0:print/tex-dvipdfmx \
		tex-formats>0:print/tex-formats \
		tex-dvipsk>0:print/tex-dvipsk
.endif


do-build:
.if ${PRODUCT} == "TrueNAS"
	(cd ${WRKSRC}/userguide && make TAG="truenas" html \
		&& cd processed/_build/html && ${COPYTREE_SHARE} . ${WRKSRC}/userguide/processed_html \
		&& cd ${WRKSRC}/userguide && make TAG="truenas" pdf)
.else
	(cd ${WRKSRC}/userguide && make html)
.endif

do-install:
	${MKDIR} -p ${STAGEDIR}${PREFIX}/www/data/
.if ${PRODUCT} == "TrueNAS"
	${MV} ${WRKSRC}/userguide/processed_html ${STAGEDIR}${PREFIX}/www/data/docs
	${CP} -p ${WRKSRC}/userguide/processed/_build/latex/TrueNAS.pdf ${STAGEDIR}${PREFIX}/www/data/docs/
.else
	${MV} ${WRKSRC}/userguide/processed/_build/html ${STAGEDIR}${PREFIX}/www/data/docs
.endif
	(cd ${STAGEDIR}${PREFIX}/www/data/docs; ${FIND} . -type f \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print "www/data/docs/"$$2 }' >> ${TMPPLIST})

.include <bsd.port.post.mk>
