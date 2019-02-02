# $FreeBSD$

.include "../Makefile.common"

PORTNAME=	kernel
PORTVERSION=	${OS_PORTVERSION}
CATEGORIES=	os

MAINTAINER=	kris@ixsystems.com
COMMENT=	Port for the OS kernel

BUILD_DEPENDS=	buildkernel>=0:os/buildkernel

PREFIX=/
EXTRACT_ONLY=
WRKSRC=/usr/src

.include <bsd.port.pre.mk>

build:
	@${MKDIR} -p ${STAGEDIR}
	@${ECHO_MSG} "==> Extracting kernel.txz..."
	@${TAR} xpf /usr/dist/kernel.txz -C ${STAGEDIR}

do-install:
	@${ECHO_MSG} "==> Generating plist..."
	@${MKDIR} -p ${STAGEDIR}
	@(cd ${STAGEDIR}; ${FIND} ./boot/kernel \( -type f -o -type l \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' >> ${TMPPLIST})
	@(cd ${STAGEDIR}; ${FIND} ./boot/kernel \( -type d \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' \
		| ${XARGS} -I '{}' echo "@dir {}" >> ${TMPPLIST})

.include <bsd.port.post.mk>
