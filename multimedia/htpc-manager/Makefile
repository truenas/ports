# Created by: Zackary Welch <zwelch@ixsystems.com>
# $FreeBSD$

PORTNAME=	htpc-manager
PORTVERSion=	0.11.0
PORTREVISION=	1
CATEGORIES=	multimedia

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Python based web application to manage the software on an HTPC

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/LICENSE

USES=	python:2.7
USE_PYTHON=	autoplist distutils
NO_ARCH=	yes

USE_GITHUB=	yes
GH_ACCOUNT=	styxit
GH_PROJECT=	HTPC-Manager
GH_TAGNAME=	490697460b4fa1797106aece27d873bc256b2ff1

.include <bsd.port.options.mk>

post-extract:
	${INSTALL_SCRIPT} ${FILESDIR}/setup.py ${WRKSRC}

.include <bsd.port.mk>
