# $FreeBSD$
#
# Handle dependency on the security/openssl port
#
# Feature:	portssl
# Usage:	USES=portssl
# Valid ARGS:	build (default), run
#
# MAINTAINER:	miwi@ixsystems.com

.if !defined(_INCLUDE_USES_PORTSSL_MK)
_INCLUDE_USES_PORTSSL_MK=	yes

.if empty(portssl_ARGS)
portssl_ARGS=	build
.endif

.if ${portssl_ARGS:Mbuild}
BUILD_DEPENDS+=	${LOCALBASE}/lib/libcrypto.so:security/openssl111
.endif
.if ${portssl_ARGS:Mrun}
RUN_DEPENDS+=	${LOCALBASE}/lib/libcrypto.so:security/openssl111
.endif

OPENSSLBASE=            ${LOCALBASE}
OPENSSLRPATH=		${LOCALBASE}/lib
OPENSSLDIR?=            ${OPENSSLBASE}/openssl
OPENSSLLIB=		${OPENSSLBASE}/lib
OPENSSLINC=		${OPENSSLBASE}/include

MAKE_ENV+=		OPENSSLBASE=${OPENSSLBASE}
MAKE_ENV+=		OPENSSLDIR=${OPENSSLDIR}
MAKE_ENV+=		OPENSSLINC=${OPENSSLINC}

.if defined(OPENSSLRPATH)
.  if defined(USE_OPENSSL_RPATH)
CFLAGS+=		-Wl,-rpath,${OPENSSLRPATH}
.  endif
MAKE_ENV+=		OPENSSLRPATH=${OPENSSLRPATH}
OPENSSL_LDFLAGS+=	-Wl,-rpath,${OPENSSLRPATH}
.endif

LDFLAGS+=		${OPENSSL_LDFLAGS}

.endif
