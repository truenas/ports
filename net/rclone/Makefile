# $FreeBSD$

PORTNAME=	rclone
DISTVERSIONPREFIX=	v
DISTVERSION=	1.53.1
CATEGORIES=	net

MAINTAINER=	wg@FreeBSD.org
COMMENT=	Sync files to and from various cloud services

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/COPYING

USES=		go:modules
USE_GITHUB=	yes
GH_ACCOUNT=	ncw

PLIST_FILES=	bin/rclone

post-extract:
	cd ${WRKSRC} && go mod vendor

.include <bsd.port.mk>
