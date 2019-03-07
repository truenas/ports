# $FreeBSD$

.include "../Makefile.common"

PORTNAME=	kernel-debug
PORTVERSION=	${OS_PORTVERSION}
CATEGORIES=	os

MAINTAINER=	kris@ixsystems.com
COMMENT=	Port for the kernel debug files

BUILD_DEPENDS=	/usr/dist/kernel.txz:os/buildkernel

PREFIX=/
EXTRACT_ONLY=
DISTFILES=
WRKSRC=/usr/src
NEED_ROOT=	yes

.include <bsd.port.pre.mk>

do-build:
	@${MKDIR} -p ${STAGEDIR}
	@${ECHO_MSG} "==> Extracting kernel.txz..."
	@${TAR} xpf /usr/dist/kernel.txz -C ${STAGEDIR}

do-install:
	@${ECHO_MSG} "==> Generating plist..."
	@${MKDIR} -p ${STAGEDIR}
	@(cd ${STAGEDIR}; ${FIND} ./usr/lib/debug/boot/kernel \( -type f -o -type l \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' >> ${TMPPLIST})
	@(cd ${STAGEDIR}; ${FIND} ./usr/lib/debug/boot/kernel \( -type d \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' \
		| ${XARGS} -I '{}' echo "@dir {}" >> ${TMPPLIST})

.include <bsd.port.post.mk>
