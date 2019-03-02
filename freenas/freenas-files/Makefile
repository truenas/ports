# $FreeBSD$

PORTNAME=	freenas-files
PORTVERSION=	20190301205625
PORTREVISION=	0

CATEGORIES=	freenas
VALID_CATEGORIES+=	freenas

MAINTAINER=	dev@ixsystems.com
COMMENT=	miscellaneous files for FreeNAS

PRODUCT?=
PREFIX=/

USES= 	python
RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}snmp_passpersist>=1.3.0:net-mgmt/py-snmp_passpersist

.if exists(/usr/ports/local_source/${PORTNAME})
PORTVERSION!=	stat -f %m /usr/ports/local_source/${PORTNAME}
EXTRACT_ONLY=
WRKSRC=         /usr/ports/local_source/${PORTNAME}
.else
USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	freenas
GH_TAGNAME=	daea64e1a7afc0a2a94ffdb6a8674a9588afaf6b
.endif

# explicitly set this for the port build
FREEBSD_SRCTOP=/usr/src
PLIST_SUB=	MACHINE_ARCH=${MACHINE_ARCH}

ALL_TARGET=obj all

MAKE_JOBS_UNSAFE=yes

.include <bsd.port.pre.mk>

.if exists(/usr/ports/local_source/${PORTNAME})
checksum fetch:
	echo ${.TARGET} not needed because building direct
.endif

do-build:
	${MAKE} -C ${WRKSRC}/src/extract-tarball   obj
	${MAKE} -C ${WRKSRC}/src/extract-tarball  all
	${MAKE} -C ${WRKSRC}/src/fix_ea obj
	${MAKE} -C ${WRKSRC}/src/fix_ea all
	${MAKE} -C ${WRKSRC}/src/freenas-sysctl obj
	${MAKE} -C ${WRKSRC}/src/freenas-sysctl all
	${MAKE} -C ${WRKSRC}/src/winacl   obj
	${MAKE} -C ${WRKSRC}/src/winacl  all

do-install:
	mkdir -p ${STAGEDIR}${PREFIX}/usr/local/bin
	mkdir -p ${STAGEDIR}${PREFIX}/boot/modules

	${MAKE} -C ${WRKSRC}/src/extract-tarball  BINDIR=${STAGEDIR}${PREFIX}/usr/local/bin install
	${MAKE} -C ${WRKSRC}/src/fix_ea  BINDIR=${STAGEDIR}${PREFIX}/usr/local/bin install
	${MAKE} -C ${WRKSRC}/src/winacl  BINDIR=${STAGEDIR}${PREFIX}/usr/local/bin install
	${MAKE} -C ${WRKSRC}/src/freenas-sysctl KMODDIR=${STAGEDIR}${PREFIX}/boot/modules install
	#
	# Adding autotune
	#
	${INSTALL_SCRIPT} ${WRKSRC}/src/autotune/files/autotune.py ${STAGEDIR}${PREFIX}/usr/local/bin/autotune
	${INSTALL_SCRIPT} ${WRKSRC}/src/afpusers/files/afpusers.py ${STAGEDIR}${PREFIX}/usr/local/bin/afpusers
	#
	#
	${CP} -a ${WRKSRC}/src/freenas/ ${STAGEDIR}/
.if ${PRODUCT} == "TrueNAS"
	${RM} \
		${STAGEDIR}/boot/loader.conf \
		${STAGEDIR}/boot/loader.rc.local \
		${STAGEDIR}/etc/sysctl.conf \
		${STAGEDIR}/etc/pf.conf.block \
		${STAGEDIR}/etc/motd \
		${STAGEDIR}/etc/ix.rc.d/ix_pf_early \
		${STAGEDIR}/etc/ix.rc.d/ix_pf_late
.endif
	(cd ${STAGEDIR}; ${FIND} . \( -type f -o -type l \) \
		| ${SED} -e 's,^\./,,g' \
		| grep -v 'compat/linux/proc' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print $$2 }' >> ${TMPPLIST})

.include <bsd.port.post.mk>
