# Created by: Kris Moore <kmoore@FreeBSD.org>
# $FreeBSD$

PORTNAME=	pc-installdialog
PORTVERSION=	20190412130505
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
GH_TAGNAME=	a8548927d97bd1405d357e621cf738465f430699

.include <bsd.port.mk>
