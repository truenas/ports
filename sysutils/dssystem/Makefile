# Created by: John Hixson (jhixson@FreeBSD.org)
# $FreeBSD$

PORTNAME=	dssystem
PORTVERSION=	14.0.0.1
PORTREVISION=	3
CATEGORIES=	sysutils
MASTER_SITES=	http://www.asigra.com/updates/14.0/DS-System/14.0.0.1/
DISTNAME=	DS-System_14_0_0_1_FreeBSD

MAINTAINER=	jhixson@FreeBSD.org
COMMENT=	Award cloud backup and restore server application

ONLY_FOR_ARCHS=	amd64

RUN_DEPENDS=	ca_root_nss>0:security/ca_root_nss \
		gettext-runtime>0:devel/gettext-runtime \
		icu>0:devel/icu \
		indexinfo>0:print/indexinfo \
		libxml2>0:textproc/libxml2 \
		openldap-sasl-client>2.4.0:net/openldap24-sasl-client \
		nss-pam-ldapd-sasl>0:net/nss-pam-ldapd-sasl
		#postgresql10-client>9.6.0:databases/postgresql10-client \
		#postgresql10-server>9.6.0:databases/postgresql10-server

DISTFILE=	dssystem-14.0.0.1.txz

NO_BUILD=	yes
USES=		zip perl5

USE_RC_SUBR=	dssystem

do-install:
	${CP} ${FILESDIR}/dssystem_install.ini ${STAGEDIR}
	cd ${STAGEDIR} && pkg -r ${STAGEDIR} add -f ${WRKSRC}/${DISTFILE}
	${RM} ${FILESDIR}/dssystem_install.ini

.include <bsd.port.mk>
