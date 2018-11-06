# $FreeBSD$

PORTNAME=	falkon
DISTVERSION=	3.0.0
<<<<<<< HEAD
PORTREVISION=	5
=======
PORTREVISION=	2
>>>>>>> freebsd/master
CATEGORIES=	www
MASTER_SITES=	KDE/stable/falkon/${DISTVERSION:R}/src/

MAINTAINER=	kde@FreeBSD.org
COMMENT=	Web browser based on WebKit engine and Qt Framework

LICENSE=	GPLv3
LICENSE_FILE=	${WRKSRC}/COPYING

<<<<<<< HEAD
USES=		cmake:outsource desktop-file-utils qt:5 ssl tar:xz kde:5
USE_QT=		core dbus gui location network printsupport qml quick \
		sql webchannel webengine widgets x11extras linguist_build \
=======
USES=		cmake:outsource desktop-file-utils kde:5 qt:5 ssl tar:xz
USE_KDE=	wallet
USE_QT=		core dbus declarative gui location network printsupport \
		sql webchannel webengine widgets x11extras \
>>>>>>> freebsd/master
		buildtools_build qmake_build
USE_KDE=	ecm_build
USE_XORG=	xcb

USE_LDCONFIG=	yes

OPTIONS_DEFINE=	GNOMEKEYRING KWALLET
OPTIONS_SUB=	YES

KWALLET_USE=			KDE+=wallet

GNOMEKEYRING_CMAKE_BOOL=	BUILD_KEYRING
GNOMEKEYRING_USE=		GNOME=glib20
GNOMEKEYRING_LIB_DEPENDS=	libgnome-keyring.so:security/libgnome-keyring

.include <bsd.port.mk>
