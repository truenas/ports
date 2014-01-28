# $FreeBSD$

PORTNAME=	hv-kvp
PORTVERSION=	1.0
CATEGORIES=	kld
MASTER_SITES=	https://github.com/FreeBSDonHyper-V/Hyperv-Ports/raw/hyperv-ic-master/BIS-1.0/FreeNAS-9.2.1/ports/

MAINTAINER=	bsdic@microsoft.com
COMMENT=	Hyper-V KVP Integration Components

FETCH_ARGS=	-Fpr

NO_STAGE = yes

ONLY_FOR_ARCHS=	amd64 i386

pre-install:
	@PKG_PREFIX=${PREFIX} ${SH} ${PKGINSTALL} ${PKGNAME} PRE-INSTALL

post-install:
	@PKG_PREFIX=${PREFIX} ${SH} ${PKGINSTALL} ${PKGNAME} POST-INSTALL

post-deinstall:
	@PKG_PREFIX=${PREFIX} ${SH} ${PKGDEINSTALL} ${PKGNAME} POST-DEINSTALL

.include <bsd.port.mk>
