# Created by: ismail@enderunix.org
# $FreeBSD$

PORTNAME=	linkcheck
PORTVERSION=	1.4
PORTREVISION=	1
CATEGORIES=	www
MASTER_SITES=	http://www.inter7.com/linkcheck/

MAINTAINER=	ismail@EnderUNIX.org
COMMENT=	Checks a web site for bad links

BROKEN=		unfetchable
DEPRECATED=	Broken for more than 6 months
EXPIRATION_DATE=	2020-05-05

PLIST_FILES=	bin/linkcheck man/man1/linkcheck.1.gz

GNU_CONFIGURE=	yes

CFLAGS+=	-Wno-return-type

.include <bsd.port.mk>
