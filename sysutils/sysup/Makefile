# $FreeBSD$

PORTNAME=	sysup
DISTVERSION=    0.6.4-3	
DISTVERSIONSUFFIX= -g4bd2434 
DISTVERSIONPREFIX=	v
CATEGORIES=	sysutils

MAINTAINER=	kris@ixsystems.com
COMMENT=	System Update Utility

LICENSE=	BSD2CLAUSE

BUILD_DEPENDS=	${LOCALBASE}/bin/go:lang/go
RUN_DEPENDS=	${LOCALBASE}/sbin/beadm:sysutils/beadm

USES=		compiler

USE_GITHUB=	yes
GH_ACCOUNT=	trueos
GH_TAGNAME=     4bd2434e105

USE_RC_SUBR=	sysup

PLIST_FILES=	bin/sysup

post-extract:
	@${MKDIR} ${WRKSRC}/src/
	@${MV} ${WRKSRC}/vendor/* ${WRKSRC}/src/

do-build:
	@cd ${WRKSRC}; ${SETENV} ${MAKE_ENV} CGO_ENABLED=0 ${BUILD_ENV} GOPATH=${WRKSRC} go build \
		-o sysup

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/sysup ${STAGEDIR}${PREFIX}/bin/sysup

.include <bsd.port.pre.mk>

# golang assumes that if clang is in use, it is called "clang" and not "cc". If
# it's called "cc", go fails.
.if ${COMPILER_TYPE} == clang
BUILD_ENV=	CC=clang
.endif

.include <bsd.port.post.mk>
