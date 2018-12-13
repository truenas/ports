# $FreeBSD$

PORTNAME=	zol
PORTVERSION=	2018121200
DISTVERSIONPREFIX=	v
CATEGORIES=	sysutils

MAINTAINER=	miwi@FreeBSD.org
COMMENT=	ZFS

LICENSE=	CDDL
#LICENSE_FILE=	${WRKSRC}/COPYING

GNU_CONFIGURE=	yes
USES=		kmod uidfix 
USE_GITHUB=	yes
GH_ACCOUNT=	zfsonfreebsd
GH_PROJECT=	ZoF
GH_TAGNAME=	f92b728

do-configure:
	(cd ${WRKSRC} && ./autogen.sh)

do-build:
	(cd ${WRKSRC}/module/ && make -f Makefile.bsd)

.include <bsd.port.mk>
