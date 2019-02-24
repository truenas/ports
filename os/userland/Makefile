# $FreeBSD$

.include "../Makefile.common"

PORTNAME=	userland
PORTVERSION=	${OS_PORTVERSION}
CATEGORIES=	os

MAINTAINER=	kris@ixsystems.com
COMMENT=	Port for the OS userland binaries

BUILD_DEPENDS=  /usr/dist/world.txz:os/buildworld

PREFIX=/
EXTRACT_ONLY=
DISTFILES=

NO_OPTIONS_SORT=	yes

OPTIONS_DEFINE=	DOCS LIB32 DEBUG TESTS
OPTIONS_DEFAULT= DOCS LIB32
OPTIONS_SUB=	yes
LIB32_DESC=	Libraries to run i386 binaries on amd64 platforms
TESTS_DESC=	Test Suite Scripts

.include <bsd.port.pre.mk>

.if ${PORT_OPTIONS:MDOCS}
RUN_DEPENDS+=	userland-docs=${OS_PORTVERSION}:os/userland-docs
.endif

.if ${PORT_OPTIONS:MLIB32}
RUN_DEPENDS+=	userland-lib32=${OS_PORTVERSION}:os/userland-lib32
.endif

.if ${PORT_OPTIONS:MDEBUG}
RUN_DEPENDS+=	userland-debug=${OS_PORTVERSION}:os/userland-debug
.endif

.if ${PORT_OPTIONS:MTESTS}
RUN_DEPENDS+=	userland-tests=${OS_PORTVERSION}:os/userland-tests
.endif

do-build:
	@${MKDIR} -p ${STAGEDIR}
	@${ECHO_MSG} "==> Extracting world.txz..."
	@${TAR} xpf /usr/dist/world.txz -C ${STAGEDIR}

do-install:
	@${ECHO_MSG} "==> Generating plist..."
	@(cd ${STAGEDIR}; ${FIND} . \( -type f -o -type l \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' \
		| ${GREP} -v -e 'usr/lib/debug' -e 'usr/tests' -e 'usr/lib32' -e 'usr/share/doc' -e 'usr/share/man' \
		>> ${TMPPLIST})
	@(cd ${STAGEDIR}; ${FIND} ./ \( -type d \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' \
		| ${GREP} -v -e '/' \
		| ${GREP} -v -e 'usr/lib/debug' -e 'usr/tests' -e 'usr/lib32' -e 'usr/share/doc' -e 'usr/share/man' \
		| ${XARGS} -I '{}' echo "@dir {}" \
		| ${GREP} -v -w '@dir ' \
		| ${CAT} >> ${TMPPLIST})
	@sh ${FILESDIR}/setconfig.sh ${FILESDIR}/configs ${TMPPLIST}

clean:
	@if [ -d ${WRKDIR} ]; then \
		if [ -w ${WRKDIR} ]; then \
			${ECHO_MSG} "===> Cleaning for ${PKGNAME}"; \
			chflags -R noschg ${WRKDIR} ; \
			${RM} -r ${WRKDIR}; \
		else \
			${ECHO_MSG} "===>   ${WRKDIR} not writable, skipping"; \
		fi; \
	fi

.include <bsd.port.post.mk>
