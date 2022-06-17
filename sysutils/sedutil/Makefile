# Created by: Alexander Motin <mav@FreeBSD.org>
# $FreeBSD$

PORTNAME=	sedutil
PORTVERSION=	1.15.1
PORTREVISION=	13
CATEGORIES=	sysutils

MAINTAINER=	mav@FreeBSD.org
COMMENT=	Self Encrypting Drive Utility

LICENSE=	GPLv3

USES=		gmake
USE_GITHUB=	yes
GH_ACCOUNT=	amotin
GH_TAGNAME=	235b9c1

PLIST_FILES=	sbin/sedutil-cli man/man8/sedutil-cli.8.gz

do-build:
	(cd ${WRKSRC}/freebsd/CLI/ && gmake)

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/freebsd/CLI/dist/Release/CLang-Generic/sedutil-cli ${STAGEDIR}${PREFIX}/sbin
	${INSTALL_MAN} ${WRKSRC}/docs/sedutil-cli.8 ${STAGEDIR}${MANPREFIX}/man/man8

.include <bsd.port.mk>
