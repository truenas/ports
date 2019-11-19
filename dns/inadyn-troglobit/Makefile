# Created by: Vladimir Vinogradenko <themylogin@gmail.com>
# $FreeBSD$

PORTNAME=		inadyn-troglobit
DISTVERSION=	2.5
PORTREVISION=	2
CATEGORIES=		dns

MAINTAINER=	themylogin@gmail.com
COMMENT=	Dynamic DNS client with SSL/TLS support

LICENSE=	GPLv2

LIB_DEPENDS=libconfuse.so:devel/libconfuse \
			libgnutls.so:security/gnutls

PLIST_FILES=man/man8/inadyn.8.gz \
			man/man5/inadyn.conf.5.gz \
			share/doc/inadyn/COPYING \
			share/doc/inadyn/ChangeLog.md \
			share/doc/inadyn/README.md \
			sbin/inadyn

USES=		autoreconf libtool pkgconfig ssl
USE_GITHUB=		yes
USE_RC_SUBR=	inadyn
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--enable-openssl
CONFIGURE_ENV=	OpenSSL_CFLAGS="-I${OPENSSLINC}" OpenSSL_LIBS="-L${OPENSSLLIB} -lssl"

GH_ACCOUNT=		troglobit
GH_PROJECT=		inadyn
GH_TAGNAME=		9d3d7852991d04438079f61e8f6092cd7b86668a

.include <bsd.port.mk>
