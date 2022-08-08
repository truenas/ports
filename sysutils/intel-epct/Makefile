PORTNAME=	epct
DISTVERSION=	1.39.5.5
CATEGORIES=	sysutils
MASTER_SITES=	https://downloadmirror.intel.com/${MASTER_SITE_SUBDIR}/ \
		http://downloadmirror.intel.com/${MASTER_SITE_SUBDIR}/
MASTER_SITE_SUBDIR=	738719
PKGNAMEPREFIX=	intel-
DISTNAME=	EPCT_${DISTVERSION}

MAINTAINER=	ryan@iXsystems.com
COMMENT=	Intel(R) Ethernet Port Configuration Tool (EPCT)

LICENSE=		IPL

USES=			zip

IGNORE_DragonFly=	FreeBSD binary port
ONLY_FOR_ARCHS=		amd64
ONLY_FOR_ARCHS_REASON=	not supported on anything other than amd64

LICENSE_NAME_IPL=	Intel Proprietary License
LICENSE_FILE_IPL=	${WRKSRC}/epct.txt
LICENSE_PERMS_IPL=	dist-mirror dist-sell pkg-mirror pkg-sell auto-accept

WRKSRC=			${WRKDIR}/FreeBSDx64
NO_BUILD=		yes
NO_ARCH=		yes
NO_ARCH_IGNORE=		epct64e

SUB_FILES=	epct
SUB_LIST=	EPCT_DIR=${EPCT_DIR}

EPCT_DIR=	intel-epct

.include <bsd.port.pre.mk>

do-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/${EPCT_DIR}
	cd ${WRKSRC} && ${COPYTREE_SHARE} . ${STAGEDIR}${PREFIX}/${EPCT_DIR} "! -name 'epct'"
	cd ${WRKDIR} && ${CP} epct.txt ${STAGEDIR}${PREFIX}/${EPCT_DIR}
	${INSTALL_PROGRAM} ${WRKSRC}/epct64e ${STAGEDIR}${PREFIX}/${EPCT_DIR}
	${INSTALL_SCRIPT} ${WRKDIR}/epct ${STAGEDIR}${PREFIX}/sbin/

.include <bsd.port.post.mk>
