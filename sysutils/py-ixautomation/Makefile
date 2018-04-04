# Created by: Joe Maloney <jmaloney@ixsystems.com>
# $FreeBSD$

PORTNAME=	ixautomation
PORTVERSION=	18.04
CATEGORIES=	sysutils
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	jmaloney@ixsystems.com
COMMENT=	Automation framework for iXsystems projects

LICENSE=	BSD2CLAUSE

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}pip>0:devel/py-pip@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}pytest>=0:devel/py-pytest@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}requests>=0:www/py-requests@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}selenium>=0:www/py-selenium@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}pyvirtualdisplay>=0:x11/py-pyvirtualdisplay@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}ws4py>=0:www/py-ws4py@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}unittest-xml-reporting>=0:devel/py-unittest-xml-reporting@${FLAVOR} \
		${LOCALBASE}/sbin/vm:sysutils/vm-bhyve \
		${LOCALBASE}/bin/expect:lang/expect \
		${LOCALBASE}/bin/firefox:www/firefox-esr \
		${LOCALBASE}/bin/geckodriver:www/geckodriver \
		xorg-vfbserver>=0:x11-servers/xorg-vfbserver \
		bhyve-firmware>=0:sysutils/bhyve-firmware

WRKSRC_SUBDIR=  src
USES=		python:3.6
USE_PYTHON=	distutils
MAKE_ARGS=	PREFIX=${STAGEDIR}${PREFIX}

USE_GITHUB=	yes
GH_ACCOUNT=	ixsystems
GH_PROJECT=	ixautomation
GH_TAGNAME=	f47fa46cb4b38202ffb69ff2bc173856bbe0e43c

.include <bsd.port.mk>
