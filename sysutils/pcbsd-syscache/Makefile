# Created by: Kris Moore <kmoore@FreeBSD.org>
# $FreeBSD: head/sysutils/pcbsd-syscache/Makefile 378356 2015-02-03 15:05:14Z kmoore $

PORTNAME=	pcbsd-syscache
PORTVERSION=	1421343297
CATEGORIES=	sysutils
MASTER_SITES=	http://www.pcbsd.org/~kris/software/ \
		ftp://ftp.pcbsd.org/pub/software/

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	PC-BSD SysCache Utility

LICENSE=	BSD

WRKSRC=		${WRKDIR}/syscache
USE_QT5=	core network buildtools qmake
USES=		pkgconfig tar:xz
NO_BUILD=	yes

.include <bsd.port.pre.mk>

.if ${OSVERSION} < 1000000
IGNORE=		requires FreeBSD 10 or later
.endif

do-install:
	cd ${WRKSRC} && make install PREFIX=${STAGEDIR}${PREFIX}

post-install:
	PKG_PREFIX=${STAGEDIR}${PREFIX} ${SH} ${PKGINSTALL} ${PKGNAME} POST-INSTALL

.include <bsd.port.post.mk>
