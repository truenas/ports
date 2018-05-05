# Created by: Joe Maloney <jmaloney@ixsystems.com>
# $FreeBSD$

PORTNAME=	ixautomation
PORTVERSION=	18.05
CATEGORIES=	sysutils
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	jmaloney@ixsystems.com
COMMENT=	Automation framework for iXsystems projects

LICENSE=	BSD2CLAUSE

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}pip>0:devel/py-pip@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}pytest>=0:devel/py-pytest@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}requests>=0:www/py-requests@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}selenium>=0:www/py-selenium@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}ws4py>=0:www/py-ws4py@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}unittest-xml-reporting>=0:devel/py-unittest-xml-reporting@${FLAVOR} \
		ix-bhyve>=0:sysutils/ix-bhyve \
		${LOCALBASE}/bin/expect:lang/expect \
		bhyve-firmware>=0:sysutils/bhyve-firmware

WRKSRC_SUBDIR=  src
USES=		python:3.6
USE_PYTHON=	distutils
MAKE_ARGS=	PREFIX=${STAGEDIR}${PREFIX}

USE_GITHUB=	yes
GH_ACCOUNT=	ixsystems
GH_PROJECT=	ixautomation
GH_TAGNAME=	b2f9f54d754ea52616cdd16d5b8bcd630e772b55

.include <bsd.port.mk>
