# $FreeBSD$
#
# handle tar archives
#
# Feature:	tar
# Usage:	USES=tar[:[xz|lzma|Z|bzip2|tgz|tbz]]
#
# MAINTAINER: portmgr@FreeBSD.org

.if !defined(_INCLUDE_USES_TAR_MK)
_INCLUDE_USES_TAR_MK=	yes

<<<<<<< HEAD
=======
tar_ARGS?=	none

>>>>>>> freenas/9.2.1-BRANCH
.if ${tar_ARGS} == xz
EXTRACT_SUFX?=	.tar.xz
.elif ${tar_ARGS} == lzma
EXTRACT_SUFX?=	.tar.lzma
<<<<<<< HEAD
.elif ${tar_ARGS} == txz
EXTRACT_SUFX?=	.txz
.elif ${tar_ARGS} == bzip2 || ${tar_ARGS} == bz2
=======
.elif ${tar_ARGS} == bzip2
>>>>>>> freenas/9.2.1-BRANCH
EXTRACT_SUFX?=	.tar.bz2
.elif ${tar_ARGS} == tgz
EXTRACT_SUFX?=	.tgz
.elif ${tar_ARGS} == tbz
EXTRACT_SUFX?=	.tbz
.elif ${tar_ARGS} == Z
EXTRACT_SUFX?=	.tar.Z
<<<<<<< HEAD
.elif empty(tar_ARGS)
=======
.elif ${tar_ARGS} == none
>>>>>>> freenas/9.2.1-BRANCH
EXTRACT_SUFX?=	.tar
.else
IGNORE=	Incorrect 'USES+=tar:${tar_ARGS}'
.endif
.endif
