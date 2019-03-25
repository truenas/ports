# $FreeBSD$

PORTNAME=	manifest
PORTVERSION=	${:!/usr/bin/stat -f %Sm -t %s ${PORTSDIR}/local_source/trueos-manifest.json!}
CATEGORIES=	os

MAINTAINER=	ken@ixsystems.com
COMMENT=	Port for the latest package build manifest

PREFIX=/
NO_BUILD=	yes
EXTRACT_ONLY=
DISTFILES=
PLIST_FILES=	/var/db/current-manifest.json

.include <bsd.port.pre.mk>

do-install:
	@${MKDIR} -p ${STAGEDIR}/var/db
	@${CP} ${PORTSDIR}/local_source/trueos-manifest.json ${STAGEDIR}/var/db/current-manifest.json

.include <bsd.port.post.mk>
