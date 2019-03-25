# Created by: William Grzybowski <wg@FreeBSD.org>
# $FreeBSD$

PORTNAME=	libzfs
PORTVERSION=	20190325201720
CATEGORIES=	devel python
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	wg@FreeBSD.org
COMMENT=	Python libzfs bindings

LICENSE=	BSD2CLAUSE

BROKEN_mips=		fails to build: libzfs.c: error: convert_for_assignment: incompatible type for argument 3 of zfs_path_to_zhandle
BROKEN_mips64=		fails to build: libzfs.c: error: convert_for_assignment: incompatible type for argument 3 of zfs_path_to_zhandle
BROKEN_powerpc64=	fails to build: libzfs.c: error: convert_for_assignment: incompatible type for argument 3 of zfs_path_to_zhandle

RUN_DEPENDS=	${PY_ENUM34}

USE_GITHUB=     yes
GH_ACCOUNT=	freenas
GH_PROJECT=	py-libzfs
GH_TAGNAME=	396e52a1b41f64db4e9fe5c4a0a800ea7a28baac

HAS_CONFIGURE=	yes
USES=		python
USE_PYTHON=	autoplist distutils cython

# Py-libzfs needs libzfs to be built
.if !exists(/sbin/zpool)
BUILD_DEPENDS+=	zol>=0:sysutils/zol
.endif

SRC_BASE?=	/usr/src
MAKE_ENV=	FREEBSD_SRC=${SRC_BASE}

.include <bsd.port.options.mk>

.if !exists(${SRC_BASE}/sys/Makefile)
IGNORE=	requires kernel source files in ${SRC_BASE}
.endif

.include <bsd.port.mk>
