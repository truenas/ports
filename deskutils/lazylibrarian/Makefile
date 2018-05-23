# Created by: Zackary Welch <zwelch@ixsystems.com> and Joshua Ruehlig
# $FreeBSD$

PORTNAME=	lazylibrarian
PORTVERSION=	9.3.0
PORTREVISION=	2
CATEGORIES=	deskutils

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Author metadata gatherer and ebook manager

LICENSE=	GPLv3

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}sqlite3>0:databases/py-sqlite3@${PY_FLAVOR}

USES=	python:2.6-2.7
NO_ARCH=	yes

USE_GITHUB= yes
GH_ACCOUNT=	DobyTang
GH_PROJECT= LazyLibrarian
GH_TAGNAME= 8c6b66e6e5709614ccb4ea19fa022486c99c1e66

NO_BUILD=	yes
PLIST_SUB=	PORTNAME=${PORTNAME}
SUB_LIST=	PORTNAME=${PORTNAME} PYTHON_CMD=${PYTHON_CMD}
SUB_FILES=	pkg-message
USE_RC_SUBR=	${PORTNAME}

USERS=	librarian
GROUPS=	${USERS}

.include <bsd.port.pre.mk>

do-install:
	${MKDIR} ${STAGEDIR}${DATADIR}/${GH_PROJECT}
	cd ${WRKSRC} && ${COPYTREE_SHARE} . ${STAGEDIR}${DATADIR}/${GH_PROJECT}

.include <bsd.port.mk>
