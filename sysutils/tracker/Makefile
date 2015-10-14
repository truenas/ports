# Created by: Michael Johnson <ahze@FreeBSD.org>
# $FreeBSD$

PORTNAME=	tracker
PORTVERSION=	1.4.1
CATEGORIES=	sysutils gnome
MASTER_SITES=	GNOME

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Object database, tag/metadata database, search tool and indexer

LIB_DEPENDS=	libdbus-glib-1.so:${PORTSDIR}/devel/dbus-glib \
		libgmime-2.6.so:${PORTSDIR}/mail/gmime26 \
		libhal.so:${PORTSDIR}/sysutils/hal \
		libraptor.so:${PORTSDIR}/textproc/raptor \
		libuuid.so:${PORTSDIR}/misc/e2fsprogs-libuuid \
		libicutu.so:${PORTSDIR}/devel/icu \
		libmediaart-2.0.so:${PORTSDIR}/multimedia/libmediaart \
		librest-0.7.so:${PORTSDIR}/devel/librest
RUN_DEPENDS=	xdg-desktop-icon:${PORTSDIR}/devel/xdg-utils \
		o3totxt:${PORTSDIR}/converters/o3read \
		pdftotext:${PORTSDIR}/graphics/poppler-utils \
		w3m:${PORTSDIR}/www/w3m

PORTSCOUT=	limitw:1,even

USES=		gettext gmake libtool pathfix pkgconfig tar:xz
USE_LDCONFIG=	yes
USE_SQLITE=	3
GNU_CONFIGURE=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib
CONFIGURE_ARGS=	--with-session-bus-services-dir=${LOCALBASE}/share/dbus-1/services \
		--disable-libiptcdata --enable-miner-fs \
		--disable-meegotouch \
		--enable-icu-charset-detection \
		--disable-network-manager \
		--disable-enca \
		--enable-tracker-preferences \
		--disable-libosinfo
# disable evolution miner until tracker fixes the dep
CONFIGURE_ARGS+=	--disable-miner-evolution
INSTALL_TARGET=	install-strip
GLIB_SCHEMAS=	org.freedesktop.Tracker.DB.gschema.xml \
		org.freedesktop.Tracker.Extract.gschema.xml \
		org.freedesktop.Tracker.FTS.gschema.xml \
		org.freedesktop.Tracker.Miner.Files.gschema.xml \
		org.freedesktop.Tracker.Store.gschema.xml \
		org.freedesktop.Tracker.Writeback.gschema.xml \
		org.freedesktop.Tracker.enums.xml \
		org.freedesktop.Tracker.gschema.xml

PLIST_SUB=	TRACKER_VER="1.0" LIBVER=400

OPTIONS_DEFINE=		DOCUMENT_MINERS
OPTIONS_DEFAULT=	DOCUMENT_MINERS
OPTIONS_SUB=yes
# and Evolution
DOCUMENT_MINERS_DESC=	Support for some sort of documents (pdf, ps, xps, office)

DOCUMENT_MINERS_CONFIGURE_ENABLE=	libgxps poppler libxml2 libgsf \
				abiword dvi ps exempi text
DOCUMENT_MINERS_LIB_DEPENDS=	libpoppler-glib.so:${PORTSDIR}/graphics/poppler-glib \
				libgxps.so:${PORTSDIR}/graphics/libgxps \
				libgsf-1.so:${PORTSDIR}/devel/libgsf \
				libexempi.so:${PORTSDIR}/textproc/exempi \
				libwv.so:${PORTSDIR}/textproc/wv

.include <bsd.port.mk>
