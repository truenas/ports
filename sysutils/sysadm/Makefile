# $FreeBSD$

PORTNAME=	sysadm
<<<<<<< HEAD
PORTVERSION=	201806120928
=======
PORTVERSION=	1.1
DISTVERSIONPREFIX=	v
PORTREVISION=	1
>>>>>>> freebsd/master
CATEGORIES=	sysutils

MAINTAINER=	jt@ixsystems.com
COMMENT=	SysAdm API server

LICENSE=	BSD3CLAUSE

WRKSRC_SUBDIR=	src
USE_QT=		concurrent core network buildtools qmake gui websockets sql
USES=		pkgconfig qt:5 tar:xz qmake ssl
MAKE_ARGS=	PREFIX=${STAGEDIR}${PREFIX}

USE_GITHUB=	yes
GH_ACCOUNT=	trueos
GH_PROJECT=	sysadm
GH_TAGNAME=	aab6f484ef01c893ae1701f64732254855eb3cdd

.include <bsd.port.mk>
