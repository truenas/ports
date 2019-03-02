# $FreeBSD$

PORTNAME=	freenas-installer
PORTVERSION=	20190301205625
PORTREVISION=	0

CATEGORIES=	freenas
VALID_CATEGORIES+=	freenas

MAINTAINER=	dev@ixsystems.com
COMMENT=	FreeNAS installer

PRODUCT?=
USE_XZ=		yes
NO_BUILD=	yes

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

PREFIX=/

# explicitly set this for the port build
PLIST_SUB=	MACHINE_ARCH=${MACHINE_ARCH}

pre-clean:
	@

ALL_TARGET=obj all

MAKE_JOBS_UNSAFE=yes

.include <bsd.port.pre.mk>

.if ${PRODUCT} == "TrueNAS"
PLIST_SUB+=	TRUENAS=""
.else
PLIST_SUB+=	TRUENAS="@comment "
.endif

.if exists(/usr/ports/local_source/${PORTNAME})
checksum fetch:
	echo ${.TARGET} not needed because building direct
.endif

do-build:

do-install:
	mkdir -p ${STAGEDIR}/${PREFIX}/pre-install
	${INSTALL_SCRIPT} ${WRKSRC}/src/freenas-installer/bin/install_worker.sh ${STAGEDIR}/${PREFIX}/bin/install_worker.sh
	${INSTALL_SCRIPT} ${WRKSRC}/src/freenas-installer/bin/install_worker2.sh ${STAGEDIR}/${PREFIX}/bin/install_worker2.sh
	${INSTALL_SCRIPT} ${WRKSRC}/src/freenas-installer/etc/install.sh ${STAGEDIR}/${PREFIX}/etc/install.sh
	${INSTALL_SCRIPT} ${WRKSRC}/src/freenas-installer/etc/rc ${STAGEDIR}/${PREFIX}/etc/rc
	${INSTALL_SCRIPT} ${WRKSRC}/src/freenas-installer/lib/functions.sh ${STAGEDIR}/${PREFIX}/lib/functions.sh
	cp -a ${WRKSRC}/src/freenas-installer/pre-install/ ${STAGEDIR}/${PREFIX}/pre-install/
.if ${PRODUCT} == "TrueNAS"
	${INSTALL_SCRIPT} /usr/truenas/src/freenas-installer/etc/install_sata_dom.sh ${STAGEDIR}/${PREFIX}/etc/install_sata_dom.sh
	cp -a /usr/truenas/src/freenas-installer/pre-install/ ${STAGEDIR}/${PREFIX}/pre-install/
.endif

.include <bsd.port.post.mk>
