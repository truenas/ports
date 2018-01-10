# Created by: Edward Tomasz Napierala <trasz@FreeBSD.org>
# $FreeBSD$

PORTNAME=	libsunacl
PORTVERSION=	1.0
CATEGORIES=	sysutils

MAINTAINER=	john@ixsystems.com
COMMENT=	Wrapper providing SunOS NFSv4 ACL API

LICENSE=	BSD2CLAUSE

NO_CHECKSUM=	yes
WRKSRC=	/usr/libsunacl_src

USES=		uidfix
USE_LDCONFIG=	yes
PLIST_FILES=	include/sunacl.h lib/libsunacl.a lib/libsunacl.so.1 lib/libsunacl.so
MAKE_ENV=	NO_PROFILE=yes

checksum extract fetch:
	${ECHO_CMD} ${.TARGET} not needed because building direct

.include <bsd.port.mk>
