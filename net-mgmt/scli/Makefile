# Created by: Michael Lyngbøl <lyngbol@wheel.dk>
# $FreeBSD$

PORTNAME=	scli
PORTVERSION=	0.3.1
PORTREVISION=	6
CATEGORIES=	net-mgmt
MASTER_SITES=	ftp://ftp.ibr.cs.tu-bs.de/local/${PORTNAME}/ \
		http://matt.peterson.org/FreeBSD/ports/

MAINTAINER=	lyngbol@wheel.dk
COMMENT=	A collection of SNMP command line management tools

BROKEN=		unfetchable
DEPRECATED=	Broken for more than 6 months
EXPIRATION_DATE=	2020-05-05

LIB_DEPENDS=	libgsnmp.so:net-mgmt/gsnmp

GNU_CONFIGURE=	yes
USES=		gmake gnome ncurses pkgconfig readline
USE_GNOME=	libxml2

INFO=		scli
PLIST_FILES=	bin/scli man/man1/scli.1.gz

.include <bsd.port.mk>
