# Created by: stas
# $FreeBSD$

PORTNAME=	libinotify
<<<<<<< HEAD
PORTVERSION=	20140622
=======
PORTVERSION=	20120419
PORTREVISION=	2
>>>>>>> freenas/9.2.1-BRANCH
CATEGORIES=	devel

MAINTAINER=	delphij@FreeBSD.org
COMMENT=	Kevent based inotify compatible library

LICENSE=	MIT

USE_GITHUB=	yes
GH_ACCOUNT=	dmatveev
GH_PROJECT=	libinotify-kqueue
<<<<<<< HEAD
GH_COMMIT=	d775062
=======
GH_COMMIT=	2a0d601
>>>>>>> freenas/9.2.1-BRANCH
GH_TAGNAME=	${GH_COMMIT}

USE_LDCONFIG=	yes

post-patch:
	@${CP} ${FILESDIR}/Makefile ${WRKSRC}
	@${TOUCH} ${WRKSRC}/config.h

.include <bsd.port.mk>
