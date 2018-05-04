# $FreeBSD$

PORTNAME=	ix-bhyve
PORTVERSION=	18.05
PORTREVISION=	1
CATEGORIES=	sysutils

MAINTAINER=	jmaloney@ixsystems.com
COMMENT=	Fork of vm-bhyve for ixautomation

LICENSE=	BSD2CLAUSE
LICENSE_FILE=	${WRKSRC}/LICENSE

RUN_DEPENDS=	${LOCALBASE}/share/certs/ca-root-nss.crt:security/ca_root_nss

CONFLICTS=	vm-bhyve-[0-9]*

OPTIONS_DEFINE=	EXAMPLES
EXAMPLES_DESC=	Install example guest templates

USE_GITHUB=	yes
GH_ACCOUNT=	ixsystems

NO_ARCH=	yes
NO_BUILD=	yes

do-install:
	${INSTALL_SCRIPT} ${WRKSRC}/vm ${STAGEDIR}${PREFIX}/sbin
	${INSTALL_SCRIPT} ${WRKSRC}/rc.d/vm ${STAGEDIR}${PREFIX}/etc/rc.d
	(cd ${WRKSRC}/lib/ && ${COPYTREE_SHARE} . ${STAGEDIR}${PREFIX}/lib/vm-bhyve)
	(cd ${WRKSRC}/sample-templates/ && ${COPYTREE_SHARE} . ${STAGEDIR}${EXAMPLESDIR})
	${INSTALL_MAN} ${WRKSRC}/vm.8 ${STAGEDIR}${MAN8PREFIX}/man/man8

.include <bsd.port.mk>
