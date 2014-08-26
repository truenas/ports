# $FreeBSD$

PORTNAME=	glusterfs
DISTVERSIONPREFIX=	freebsd_
DISTVERSION=	20140629
CATEGORIES=	sysutils
MASTER_SITES=	https://download.gluster.org/pub/gluster/experimental/

MAINTAINER=	bapt@FreeBSD.org
COMMENT=	GlusterFS distributed file system

LICENSE=	GPLv2 LGPL3
LICENSE_COMB=	dual

BUILD_DEPENDS=	git:${PORTSDIR}/devel/git # to please build-aux/pkg-version
LIB_DEPENDS=	libargp.so:${PORTSDIR}/devel/argp-standalone
.if !exists(/usr/lib/libexecinfo.so)
LIB_DEPENDS+=	libexecinfo.so:${PORTSDIR}/devel/libexecinfo
.endif

WRKSRC=		${WRKDIR}/glusterfs

USES=		bison gettext fuse libtool pkgconfig tar:bzip2
USE_LDCONFIG=	yes
USE_PYTHON=	yes
USE_GNOME=	libxml2
USE_AUTOTOOLS=	aclocal autoconf automake autoheader libtoolize

PLIST_SUB+=	RESETPREFIX=${PREFIX}

INSTALL_TARGET=	install-strip
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--with-pkgconfigdir=${PREFIX}/libdata/pkgconfig \
		--with-mountutildir=${PREFIX}/sbin \
		--localstatedir=/var
CPPFLAGS+=	-I${PREFIX}/include
LDFLAGS+=	-L${PREFIX}/lib -largp -lexecinfo
ACLOCAL_ARGS=	-I ./contrib/aclocal
AUTOMAKE_ARGS=	--add-missing --copy --foreign

post-install:
	@${MV} ${STAGEDIR}/var/db/glusterd/groups/virt \
		${STAGEDIR}/var/db/glusterd/groups/virt.sample

.include <bsd.port.mk>
