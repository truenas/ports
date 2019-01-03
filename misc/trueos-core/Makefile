# $FreeBSD$

PORTNAME=	trueos-core
PORTVERSION=	201807060938
CATEGORIES=	misc
MASTER_SITES=
DISTFILES=

MAINTAINER=	jmaloney@ixsystems.com
COMMENT=	TrueOS Core Utilities

RUN_DEPENDS=    ${LOCALBASE}/share/trueos/scripts/functions.sh:sysutils/trueos-libsh \
		${LOCALBASE}/bin/lpreserver:sysutils/life-preserver \
		${LOCALBASE}/bin/sysadm-binary:sysutils/sysadm \
		${LOCALBASE}/bin/rsync:net/rsync \
		${LOCALBASE}/bin/zfs-stats:sysutils/zfs-stats \
		${LOCALBASE}/sbin/anacron:sysutils/anacron \
		openntpd>=0:net/openntpd \
		pciutils>=0:sysutils/pciutils \
		pam_mkhomedir>=0:security/pam_mkhomedir \
		tor>=0:security/tor \
		bhyve-firmware>=0:sysutils/bhyve-firmware

CONFLICTS=	pcbsd-utils-[0-9]*
WRKSRC_SUBDIR=	package-files
MAKE_ARGS=	STAGEDIR=${STAGEDIR} TYPE=core

USE_GITHUB=     yes
GH_ACCOUNT=     trueos
GH_PROJECT=     trueos-core
GH_TAGNAME=     91f61348b8ee29fc4cc688eca4d56b669867ebf6

.include <bsd.port.mk>
