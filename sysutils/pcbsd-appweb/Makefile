# Created by: Kris Moore <kmoore@FreeBSD.org>
# $FreeBSD$

PORTNAME=	pcbsd-appweb
PORTVERSION=	1447951289
CATEGORIES=	sysutils

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	PC-BSD Web Interface

LICENSE=	BSD3CLAUSE

RUN_DEPENDS=	syscache:${PORTSDIR}/sysutils/pcbsd-syscache \
		pbi:${PORTSDIR}/sysutils/pbi-manager \
		warden:${PORTSDIR}/sysutils/warden \
		nginx:${PORTSDIR}/www/nginx

IGNORE_WITH_PHP=54
USE_PHP=	curl filter gd json session
WRKSRC_SUBDIR=	src-webui
USES=		pkgconfig tar:xz
NO_BUILD=	yes
MAKE_ARGS=	PREFIX=${STAGEDIR}${PREFIX}

USE_GITHUB=	yes
GH_ACCOUNT=	pcbsd
GH_PROJECT=	pcbsd
GH_TAGNAME=	5b116b2218f718879bcbe0f1f48454db8e1e58eb

.include <bsd.port.pre.mk>

.if ${OSVERSION} < 1000000
IGNORE=		requires FreeBSD 10 or later
.endif

.include <bsd.port.post.mk>
