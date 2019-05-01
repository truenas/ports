PORTNAME=	midcli
PORTVERSION=	20190430194622
PKGNAMEPREFIX=  ${PYTHON_PKGNAMEPREFIX}

CATEGORIES=     freenas
VALID_CATEGORIES+= freenas

MAINTAINER=     dev@ixsystems.com
COMMENT=        FreeNAS Middleware Command Line Interface

USES=		python
USE_PYTHON=	autoplist distutils

BUILD_DEPENDS=	${PYTHON_PKGNAMEPREFIX}fastentrypoints>0:devel/py-fastentrypoints@${FLAVOR}
RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}middlewared>0:freenas/py-middlewared@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}ply>0:devel/py-ply@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}prompt_toolkit>0:devel/py-prompt_toolkit@${FLAVOR}

.if exists(/usr/ports/local_source/${PORTNAME})
PORTVERSION!=	stat -f %m /usr/ports/local_source/${PORTNAME}
EXTRACT_ONLY=
WRKSRC=		/usr/ports/local_source/${PORTNAME}
.else
USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	midcli
GH_TAGNAME=	f9d13c65816e0fd74ad75611a4781e4a168a26a5
.endif

.if exists(/usr/ports/local_source/${PORTNAME})
checksum fetch:
	echo ${.TARGET} not needed because building direct
.endif

.include <bsd.port.mk>
