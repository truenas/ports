# Created by Zackary Welch <zwelch@ixsystems.com>
# $FreeBSD$

PORTNAME=	headphones
PORTVERSION=	0.5.19
PORTREVISION=	1
CATEGORIES=	audio

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Automated music downloader for NZB and Torrent

LICENSE=	GPLv3
LICENSE_FILE=	${WRKSRC}/LICENSE

RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}pytz>0:devel/py-pytz@${PY_FLAVOR}

USES=	python:2.7
USE_PYTHON=	autoplist distutils
NO_ARCH=	yes

USE_GITHUB=	yes
GH_ACCOUNT=	rembo10
GH_PROJECT=	headphones
GH_TAGNAME=	3b48f22bd9615b92928804ecd2f67e6fc19ce478	

.include <bsd.port.options.mk>

post-extract:
	${INSTALL_SCRIPT} ${FILESDIR}/setup.py ${WRKSRC}

.include <bsd.port.mk>
