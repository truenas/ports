# $FreeBSD$

PORTNAME=	uefi-edk2-bhyve-devel
DISTVERSIONPREFIX=	v
DISTVERSION=	g20190401
CATEGORIES=	sysutils

MAINTAINER=	araujo@FreeBSD.org
COMMENT=	UEFI-EDK2 firmware for bhyve

LICENSE=	BSD2CLAUSE

ONLY_FOR_ARCHS=	amd64

BUILD_DEPENDS=	${BASH_CMD}:shells/bash \
		nasm:devel/nasm \
		${PYTHON_LIBDIR}/lib-dynload/_sqlite3.so:databases/py-sqlite3@${PY_FLAVOR}

USES=		gmake \
		python:2.7,build
USE_GCC=	8+
USE_GITHUB=	yes
GH_ACCOUNT=	freebsd
GH_PROJECT=	uefi-edk2
GH_TAGNAME=	4c7c2a5
GH_TUPLE=	openssl:openssl:OpenSSL_1_1_0j:openssl/src/github.com/openssl/openssl

PLIST_FILES=	${PREFIX}/share/uefi-firmware/BHYVE_UEFI_CODE${PLIST_SUFFIX}-devel.fd \
		${PREFIX}/share/uefi-firmware/BHYVE_UEFI_VARS${PLIST_SUFFIX}-devel.fd

OPTIONS_DEFINE=	DEBUG HTTP_BOOT
OPTIONS_DEFAULT=	HTTP_BOOT

DEBUG_DESC=	Build a debug configuration
HTTP_BOOT_DESC=	Build with HTTP(S) boot support

DEBUG_VARS=	uefi_target=DEBUG
DEBUG_VARS_OFF=	uefi_target=RELEASE
HTTP_BOOT_VARS=	build_args+=-DHTTP_BOOT_ENABLE=TRUE

MAKE_ARGS+=	EXTRA_LDFLAGS=-static

BASH_CMD=	${LOCALBASE}/bin/bash

BUILD_ARGS=	-DDEBUG_ON_SERIAL_PORT=TRUE

.include <bsd.port.options.mk>

post-extract:
	@${RMDIR} ${WRKSRC}/CryptoPkg/Library/OpensslLib/openssl
	@${MV} ${WRKSRC}/src/github.com/openssl/openssl \
		${WRKSRC}/CryptoPkg/Library/OpensslLib/openssl
do-build:
	${MKDIR} ${BUILD_WRKSRC}/BaseTools/Bin/FreeBSD-${MACHINE};
	${LN} -sf ${LOCALBASE}/bin/gcc${GCC_DEFAULT} ${BUILD_WRKSRC}/BaseTools/Bin/FreeBSD-${MACHINE}/gcc
	${LN} -sf ${LOCALBASE}/bin/gcc${GCC_DEFAULT} ${BUILD_WRKSRC}/BaseTools/Bin/FreeBSD-${MACHINE}/cc
	${LN} -sf ${LOCALBASE}/bin/g++${GCC_DEFAULT} ${BUILD_WRKSRC}/BaseTools/Bin/FreeBSD-${MACHINE}/g++
	${LN} -sf ${LOCALBASE}/bin/ld ${BUILD_WRKSRC}/BaseTools/Bin/FreeBSD-${MACHINE}/ld
	${LN} -sf ${LOCALBASE}/bin/gmake ${BUILD_WRKSRC}/BaseTools/Bin/FreeBSD-${MACHINE}/make
	${LN} -sf ${LOCALBASE}/bin/gcc-ar${GCC_DEFAULT} ${BUILD_WRKSRC}/BaseTools/Bin/FreeBSD-${MACHINE}/gcc-ar
	${LN} -sf ${LOCALBASE}/bin/gcc-nm${GCC_DEFAULT} ${BUILD_WRKSRC}/BaseTools/Bin/FreeBSD-${MACHINE}/gcc-nm
	${LN} -sf ${LOCALBASE}/bin/objcopy ${BUILD_WRKSRC}/BaseTools/Bin/FreeBSD-${MACHINE}/objcopy
	${LN} -sf ${LOCALBASE}/bin/nasm ${BUILD_WRKSRC}/BaseTools/Bin/FreeBSD-${MACHINE}/nasm
	unset ARCH; unset MAKEFLAGS
	PYTHON_COMMAND=${PYTHON_CMD} ${BASH_CMD} -c "cd ${WRKSRC}; . ./edksetup.sh; ${MAKE_CMD} ${MAKE_ARGS} -C ${WRKSRC}/BaseTools"
	${BASH_CMD} -c " \
		cd ${BUILD_WRKSRC}; \
		unset ARCH; unset MAKEFLAGS; \
		. ./edksetup.sh; \
		build -t GCC5 -a X64 -b ${UEFI_TARGET} -p OvmfPkg/OvmfPkgX64.dsc ${BUILD_ARGS} \
	"

do-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/share/uefi-firmware/
	${INSTALL} ${BUILD_WRKSRC}/Build/OvmfX64/${UEFI_TARGET}_GCC5/FV/OVMF_CODE.fd \
		${STAGEDIR}${PREFIX}/share/uefi-firmware/BHYVE_UEFI_CODE${PLIST_SUFFIX}-devel.fd
	${INSTALL} ${BUILD_WRKSRC}/Build/OvmfX64/${UEFI_TARGET}_GCC5/FV/OVMF_VARS.fd \
		${STAGEDIR}${PREFIX}/share/uefi-firmware/BHYVE_UEFI_VARS${PLIST_SUFFIX}-devel.fd

.include <bsd.port.mk>
RUN_DEPENDS:=	${RUN_DEPENDS:Ngcc*}
