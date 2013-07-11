# New ports collection makefile for:	py-django-simple-captcha
# Date created:	July 10, 2013
# Whom:	John Hixson <john@ixsystems.com>
# $FreeBSD$
#

PORTNAME=	django-simple-captcha
PORTVERSION=	0.3.8
CATEGORIES=	www python
MASTER_SITES=	https://pypi.python.org/packages/source/d/django-simple-captcha/
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	john@ixsystems.com
COMMENT=	A very simple, yet powerful, Django captcha application

LICENSE=	MIT

BUILD_DEPENDS+=	${PYTHON_PKGNAMEPREFIX}imaging>0:${PORTSDIR}/graphics/py-imaging
RUN_DEPENDS=	${PYTHON_SITELIBDIR}/django/bin/django-admin.py:${PORTSDIR}/www/py-django

USE_PYTHON=	yes
USE_PYDISTUTILS=	easy_install
PLIST_FILES=	%%PYTHON_SITELIBDIR%%/%%PYEASYINSTALL_EGG%%

.include <bsd.port.mk>
