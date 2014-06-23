# $FreeBSD$

PORTNAME=	glusterfs
DISTVERSIONPREFIX=	freebsd_
DISTVERSION=	20140623
CATEGORIES=	sysutils
MASTER_SITES=	https://download.gluster.org/pub/gluster/experimental/

MAINTAINER=	root@localhost
COMMENT=	GlusterFS

WRKSRC=		${WRKDIR}/glusterfs

USES+=		tar:bzip2
USE_LDCONFIG=	yes

# Links against libpython, installs gluster package
USE_PYTHON=	yes
USE_PYTHON_BUILD=	yes

# And libintl
USES+=		gettext:lib

# Links libxml2
USE_GNOME=	libxml2

# Need to fix up paths for pkgconfig files
CONFIGURE_ARGS+=	--with-pkgconfigdir=${PREFIX}/libdata/pkgconfig

# Uses fuse
USES+=		fuse

# This is tricky enough that I gave up trying to USE_AUTOTOOLS natively;
# this just makes sure they're all in place and uses the upstream
# autogen.sh
USES+=		libtool bison:build pkgconfig
USE_AUTOTOOLS=	aclocal:env autoconf:env automake:env autoheader:env libtoolize:env
GNU_CONFIGURE=	yes

CFLAGS+=        -I${LOCALBASE}/include
CPPFLAGS+=        -I${LOCALBASE}/include

pre-configure:
	(cd ${CONFIGURE_WRKSRC} && ${SETENV} ${AUTOTOOLS_ENV} ./autogen.sh) 

.include <bsd.port.mk>
