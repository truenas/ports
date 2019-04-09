# Created by: Kris Moore <kmoore@FreeBSD.org>
# $FreeBSD$

PORTNAME=	pc-installdialog
PORTVERSION=	20190408194329
CATEGORIES=	sysutils

MAINTAINER=	kmoore@FreeBSD.org
COMMENT=	TrueOS Text Installer

LICENSE=	BSD2CLAUSE

RUN_DEPENDS=	${LOCALBASE}/bin/jq:textproc/jq

USES=		pkgconfig tar:xz
MAKE_ARGS=	PREFIX=${STAGEDIR}${PREFIX}

PLIST_FILES+=	sbin/pc-installdialog

USE_GITHUB=	yes
GH_ACCOUNT=	trueos
GH_PROJECT=	pc-installdialog
GH_TAGNAME=	d3a14755ddb47a21d8bde6a17e6db3c385fdd942

.include <bsd.port.mk>
