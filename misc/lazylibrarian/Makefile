#	Created by: Zackary Welch <zwelch@ixsystems.org>
# $FreeBSD$

PORTNAME=	lazylibrarian
PORTVERSION=	20180522
CATEGORIES=	misc

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Author metadata gatherer and ebook manager

LICENSE=	GPLv3

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}sqlite3>0:databases/py-sqlite3@${PY_FLAVOR} \
	${PYTHON_PKGNAMEPREFIX}cherrypy>0:www/py-cherrypy@${PY_FLAVOR}

USES=	python:2.7
USE_PYTHON=	distutils autoplist
NO_ARCH=	yes

USE_GITHUB= yes
GH_ACCOUNT=	lazylibrarian
GH_PROJECT= LazyLibrarian
GH_TAGNAME= 75b4a5730650bac9a2fe3bc8713252a2b9e00dda

.include <bsd.port.options.mk>

post-extract:
	${INSTALL_SCRIPT} ${FILESDIR}/setup.py ${WRKSRC}

.include <bsd.port.mk>
