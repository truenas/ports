# $FreeBSD$

PORTNAME=	trident-installer
PORTVERSION=	201807260755
CATEGORIES=	trident

MAINTAINER=	ken@ixsystems.com
COMMENT=	Graphical ISO installer for Project Trident

LICENSE=	BSD2CLAUSE

RUN_DEPENDS=	lumina-coreutils>=0:x11/lumina-coreutils \
		trident-icons>=0:trident/trident-icons \
		cursor-chameleon-pearl>=0:x11-themes/cursor-chameleon-pearl \
		xinit>=0:x11/xinit \
		/boot/modules/drm.ko:graphics/drm-next-kmod \
		xorg-minimal>=0:x11/xorg-minimal \
		xf86-video-scfb>=0:x11-drivers/xf86-video-scfb

USES=	qmake qt:5
USE_QT=	core gui widgets svg concurrent \
		buildtools_build linguist_build
USE_GITHUB=	yes
GH_ACCOUNT=	project-trident
GH_PROJECT=	trident-installer
GH_TAGNAME=	ae8763f8b91b376feb94398a4e0503a8a01d2462
WRKSRC_SUBDIR=	src-qt5

.include <bsd.port.mk>
