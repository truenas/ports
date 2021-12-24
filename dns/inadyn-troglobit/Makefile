# Created by: Vladimir Vinogradenko <themylogin@gmail.com>
# $FreeBSD$

PORTNAME=      inadyn-troglobit
DISTVERSION=   2.9.1
CATEGORIES=    dns

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

USES=		autoreconf libtool pkgconfig
USE_GITHUB=		yes
USE_RC_SUBR=	inadyn
GNU_CONFIGURE=	yes
CONFIGURE_ARGS+=--enable-openssl
CONFIGURE_ENV+=	OpenSSL_CFLAGS="-I/usr/include"	OpenSSL_LIBS="-L/usr/lib -lssl -lcrypto"

GH_ACCOUNT=		troglobit
GH_PROJECT=		inadyn
GH_TAGNAME=     3bd59d553f9db7d73fc1ccef79b5be077abf3437

.include <bsd.port.mk>
