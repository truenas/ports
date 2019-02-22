PORTNAME=	middlewared
PORTVERSION=	20190221212605
PORTREVISION=	0
PKGNAMEPREFIX=  ${PYTHON_PKGNAMEPREFIX}

CATEGORIES=     freenas
VALID_CATEGORIES+= freenas

MAINTAINER=     dev@ixsystems.com
COMMENT=        FreeNAS Middleware Daemon

USES=		python
USE_PYTHON=	autoplist distutils

BUILD_DEPENDS=	${PYTHON_PKGNAMEPREFIX}fastentrypoints>0:devel/py-fastentrypoints@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}Babel>0:devel/py-babel@${PY_FLAVOR}
RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}ws4py>0:www/py-ws4py@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}aiohttp>0:www/py-aiohttp@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}aiohttp-wsgi>0:www/py-aiohttp-wsgi@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}Flask>0:www/py-flask@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}boto3>0:www/py-boto3@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}netif>0:net/py-netif@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}bsd>0:freenas/py-bsd@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}raven>0:devel/py-raven@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}markdown>0:textproc/py-markdown@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}mako>0:textproc/py-mako@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}psutil>0:sysutils/py-psutil@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}libzfs>0:devel/py-libzfs@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}netsnmpagent>0:net/py-netsnmpagent@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}pydevd>0:devel/py-pydevd@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}zeroconf>0:net/py-zeroconf@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}influxdb>0:databases/py-influxdb@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}asyncssh>0:security/py-asyncssh@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}croniter>0:sysutils/py-croniter@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}bidict>0:devel/py-bidict@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}markdown2>0:textproc/py-markdown2@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}acme>0:security/py-acme@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}josepy>0:security/py-josepy@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}certbot-dns-cloudflare>0:security/py-certbot-dns-cloudflare@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}certbot-dns-digitalocean>0:security/py-certbot-dns-digitalocean@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}certbot-dns-google>0:security/py-certbot-dns-google@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}certbot-dns-ovh>0:security/py-certbot-dns-ovh@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}zettarepl>0:sysutils/py-zettarepl@${FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}pandas>0:math/py-pandas@${PY_FLAVOR} \
		rclone>0:net/rclone \
		sysup>0:sysutils/sysup \
		ifstat>0:net/ifstat \
		swagger-ui>0:freenas/swagger-ui

USE_RC_SUBR=	middlewared

WRKSRC_SUBDIR=	src/middlewared

.if exists(/usr/ports/local_source/${PORTNAME})
PORTVERSION!=	stat -f %m /usr/ports/local_source/${PORTNAME}
EXTRACT_ONLY=
WRKSRC=		/usr/ports/local_source/${PORTNAME}
.else
USE_GITHUB=	yes
GH_ACCOUNT=	freenas
GH_PROJECT=	freenas
GH_TAGNAME=	d8773ddc099db7f1b1da8e82bcc3b3add0fc4901
.endif

.if exists(/usr/ports/local_source/${PORTNAME})
checksum fetch:
	echo ${.TARGET} not needed because building direct
.endif

.include <bsd.port.mk>
