# Created by: rflynn@acsalaska.net
# $FreeBSD$

PORTNAME=	nss-pam-ldapd
PKGNAMESUFFIX=	-sasl

MAINTAINER=	zi@FreeBSD.org
COMMENT=	Advanced fork of nss_ldap with sasl support

LICENSE=	# pet portlint

OPTIONS_SLAVE+=	SASL
LDFLAGS+=       -ljansson
BUILD_DEPENDS+= jansson>=2.10:devel/jansson
RUN_DEPENDS+=   jansson>=2.10:devel/jansson

SLAVE_PORT=	sasl # In case we come up with more
MASTERDIR=	${.CURDIR}/../nss-pam-ldapd

.include "${MASTERDIR}/Makefile"
