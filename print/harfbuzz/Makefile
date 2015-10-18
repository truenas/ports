# Created by: Koop Mast <kwm@FreeBSD.org>
# $FreeBSD$

PORTNAME=	harfbuzz
PORTVERSION=	0.9.28
CATEGORIES=	print
MASTER_SITES=	http://www.freedesktop.org/software/harfbuzz/release/

MAINTAINER=	kwm@FreeBSD.org
COMMENT=	OpenType text shaping engine

LIB_DEPENDS=	libfreetype.so:${PORTSDIR}/print/freetype2 \
		libicudata.so:${PORTSDIR}/devel/icu \
		libgraphite2.so:${PORTSDIR}/graphics/graphite2

USE_LDCONFIG=	yes
<<<<<<< HEAD
USES=		gmake libtool:keepla pathfix pkgconfig tar:bzip2
USE_GNOME=	cairo glib20
=======
USES=		gmake pathfix pkgconfig
>>>>>>> freenas/9.2.1-BRANCH

USES+=		perl5
USE_PERL5=	build

<<<<<<< HEAD
=======
USE_GNOME=	cairo glib20 ltverhack
>>>>>>> freenas/9.2.1-BRANCH
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--with-graphite2
INSTALL_TARGET=	install-strip

.include <bsd.port.pre.mk>

.if ${OSVERSION} < 1000024
EXTRA_PATCHES=	${PATCHDIR}/extra-src_hb-open-type-private.hh
.endif

.include <bsd.port.post.mk>
