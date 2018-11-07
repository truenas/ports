# $FreeBSD$

PORTNAME=	dssystem
PORTVERSION=	14.1
PORTREVISION=	2
CATEGORIES=	sysutils
MASTER_SITES=	http://builds.ixsystems.com/ix-iso/asigra/14.1-20190301/
DISTFILES=	dssystem-14.1.txz

MAINTAINER=	wg@FreeBSD.org
COMMENT=	Award cloud backup and restore server application

ONLY_FOR_ARCHS=	amd64

RUN_DEPENDS=	ca_root_nss>0:security/ca_root_nss \
		icu>0:devel/icu \
		indexinfo>0:print/indexinfo \
		libxml2>0:textproc/libxml2 \
		openldap-sasl-client>2.4.0:net/openldap24-sasl-client \
		nss-pam-ldapd-sasl>0:net/nss-pam-ldapd-sasl \
		postgresql10-client>9.6.0:databases/postgresql10-client \
		postgresql10-server>9.6.0:databases/postgresql10-server \
		postgresql10-contrib>9.6.0:databases/postgresql10-contrib \
		postgresql10-plpython>9.6.0:databases/postgresql10-plpython


NO_BUILD=	yes
USES=		gettext-runtime perl5

USE_RC_SUBR=	dssystem

EXTRACT_AFTER_ARGS=	-C ${STAGEDIR}

pre-extract:
	${MKDIR} ${STAGEDIR}

do-install:
	${CP} ${FILESDIR}/dssystem_install.ini ${STAGEDIR}
	(cd ${STAGEDIR}${PREFIX}/ds-system; ${FIND} . \( -type f -o -type l \) \
		| ${SED} -e 's,^\./,,g' \
		| ${AWK} '{print length, $$0}' | ${SORT} -rn \
		| ${AWK} '{print "ds-system/"$$2 }' >> ${TMPPLIST})

.include <bsd.port.mk>
