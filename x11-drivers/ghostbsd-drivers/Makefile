#.Created.by:.Eric Turgeon.<eturgeon@ixsystems.com>
# $FreeBSD$

PORTNAME=	ghostbsd-drivers
PORTVERSION=	18.12
CATEGORIES=	x11-drivers

MAINTAINER=	eturgeon@ixsystems.com
COMMENT=	GhostBSD default pkg
USES=		metaport

RUN_DEPENDS=	Xorg:x11/xorg-minimal \
		xf86-input-joystick>0:x11-drivers/xf86-input-joystick \
		xf86-input-keyboard>0:x11-drivers/xf86-input-keyboard \
		xf86-input-mouse>0:x11-drivers/xf86-input-mouse \
		xf86-input-synaptics>0:x11-drivers/xf86-input-synaptics \
		xf86-input-vmmouse>0:x11-drivers/xf86-input-vmmouse \
		xf86-video-amdgpu>0:x11-drivers/xf86-video-amdgpu \
		xf86-video-ati>0:x11-drivers/xf86-video-ati \
		xf86-video-intel>0:x11-drivers/xf86-video-intel \
		xf86-video-nv>0:x11-drivers/xf86-video-nv \
		xf86-video-scfb>0:x11-drivers/xf86-video-scfb \
		xf86-video-vesa>0:x11-drivers/xf86-video-vesa \
		xf86-video-vmware>0:x11-drivers/xf86-video-vmware \
		libva-intel-driver>0:multimedia/libva-intel-driver \
		libva-vdpau-driver>0:multimedia/libva-vdpau-driver \
		bwi-firmware-kmod>0:net/bwi-firmware-kmod \
		bwn-firmware-kmod>0:net/bwn-firmware-kmod


.include <bsd.port.mk>
