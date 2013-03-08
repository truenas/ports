# New ports collection makefile for:	bsdstats
# Date created:		5 August 2006
# Whom:			Marc G. Fournier <scrappy@freebsd.org>
#
# $FreeBSD$
#

PORTNAME=	bsdstats
PORTVERSION=	5.5
PORTREVISION=	3
CATEGORIES=	sysutils
DISTFILES=

MAINTAINER=	bsdstats@nanoman.ca
COMMENT=	Monthly script for reporting anonymous statistics about your machine

NO_BUILD=	yes
USE_RC_SUBR=	bsdstats
SUB_FILES=	300.statistics pkg-message

do-install:
	${MKDIR} ${PREFIX}/etc/periodic/monthly
	${INSTALL_SCRIPT} ${WRKDIR}/300.statistics ${PREFIX}/etc/periodic/monthly

.include <bsd.port.mk>
