<<<<<<< HEAD
=======
# Created by: jpaetzel@FreeBSD.org
>>>>>>> freenas/9.2.1-BRANCH
# $FreeBSD$

PORTNAME=	pybonjour
PORTVERSION=	1.1.1
CATEGORIES=	dns python
MASTER_SITES=	GOOGLE_CODE
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	jpaetzel@FreeBSD.org
COMMENT=	Python interface to bonjour

LICENSE=	MIT

<<<<<<< HEAD
USE_PYTHON=		yes
USE_PYDISTUTILS=	yes
PYDISTUTILS_AUTOPLIST=	yes

OPTIONS_SINGLE=			ZEROCONF
OPTIONS_SINGLE_ZEROCONF=	AVAHI MDNSRESPONDER
OPTIONS_DEFAULT=		MDNSRESPONDER
AVAHI_LIB_DEPENDS=		libdns_sd.so:${PORTSDIR}/net/avahi-libdns
MDNSRESPONDER_LIB_DEPENDS=	libdns_sd.so:${PORTSDIR}/net/mDNSResponder

.include <bsd.port.mk>
=======
USE_PYTHON=	yes
USE_PYDISTUTILS=	yes
PYDISTUTILS_PKGNAME=	pybonjour

BUILD_DEPENDS += ${LOCALBASE}/lib/libdns_sd.so:${PORTSDIR}/net/mDNSResponder
RUN_DEPENDS += ${BUILD_DEPENDS}

EXAMPLESDIR=	${PREFIX}/share/examples/py-${PORTNAME}

NO_STAGE=	yes

.include <bsd.port.pre.mk>

.include <bsd.port.post.mk>
>>>>>>> freenas/9.2.1-BRANCH
