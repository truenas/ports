# Created by: Mikhail Teterin
# $FreeBSD$

PORTNAME=	gpctool
PORTVERSION=	1.0.1
PORTREVISION=	7
CATEGORIES=	x11 math graphics
MASTER_SITES=	ftp://ftp.cs.man.ac.uk/pub/toby/gpc/
DISTNAME=	gpct${PORTVERSION:S/.//g}

MAINTAINER=	mi@aldan.algebra.com
COMMENT=	Evaluation tool for the Generic Polygon Clipper library

BROKEN=		unfetchable
DEPRECATED=	Broken for more than 6 months
EXPIRATION_DATE=	2020-05-05

LIB_DEPENDS=	libgpc.so:devel/libgpc	\
		libformsGL.so:x11-toolkits/xforms

USES=		gl zip:infozip uidfix
USE_GL=		yes
PLIST_FILES=	bin/gpctool
USE_LDCONFIG=	yes
NO_CDROM=	Can't be used for profit without permission

MAKEFILE=	${FILESDIR}/BSDmakefile

EXTRACT_BEFORE_ARGS=-qoa
EXTRACT_AFTER_ARGS=-d ${WRKDIR} -x ${DISTNAME}/gpc.? ${DISTNAME}/gl.c

post-patch:
	${REINPLACE_CMD} -e 's|FALSE|false|g' \
	   ${WRKSRC}/callbacks.c ${WRKSRC}/graphics.c

.include <bsd.port.mk>
