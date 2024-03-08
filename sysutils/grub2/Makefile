# Created by: sem@FreeBSD.org
# $FreeBSD$

PORTNAME=	grub2
PORTVERSION=	2.05
PORTREVISION=	2
CATEGORIES=	sysutils
DISTNAME=	grub-${PORTVERSION}

USE_GITHUB=	yes
GH_ACCOUNT=	rhboot
GH_TAGNAME=	e7b8856f8be3292afdb38d2e8c70ad8d62a61e10

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Multiboot boot loader

LICENSE=	GPLv3

BROKEN_i386=	uses amd64 assembly

ONLY_FOR_ARCHS=	amd64 i386

BUILD_DEPENDS=	git:devel/git \
		${LOCALBASE}/bin/flex:textproc/flex \
		gsed:textproc/gsed \
		python>0:lang/python \
		help2man:misc/help2man \
		bash:shells/bash
RUN_DEPENDS=	gsed:textproc/gsed

USES=		autoreconf bison cpe gettext gmake pkgconfig:build python:build tar:xz

GNU_CONFIGURE=	yes
INFO=		grub grub-dev
MAKE_JOBS_UNSAFE=	yes
SSP_UNSAFE=	yes

CPE_PRODUCT=	grub
CPE_VENDOR=	gnu

CONFIGURE_ARGS=	--disable-werror --localedir=${PREFIX}/share/locale
CONFIGURE_ENV=	CPP="${CC} -E" \
		LEX=${LOCALBASE}/bin/flex

OPTIONS_DEFINE=	MKFONT FUSE
OPTIONS_SUB=	yes
MKFONT_DESC=	Build grub-mkfont (require freetype2)
FUSE_DESC=	Build grub-mount (require FUSE)

MKFONT_CONFIGURE_ENABLE=	grub-mkfont
MKFONT_LIB_DEPENDS=		libfreetype.so:print/freetype2
MKFONT_BUILD_DEPENDS=		${LOCALBASE}/share/fonts/dejavu/DejaVuSans.ttf:x11-fonts/dejavu

FUSE_CONFIGURE_ENABLE=		grub-mount
FUSE_LIB_DEPENDS=		libfuse.so:sysutils/fusefs-libs

.include <bsd.port.pre.mk>

.if ${ARCH} != amd64
PLIST_SUB+=	EFIEMU="@comment "
.else
PLIST_SUB+=	EFIEMU=""
.endif

post-patch:
	@${LN} -s ${LOCALBASE}/share/fonts/dejavu/DejaVuSans.ttf ${WRKSRC}
	@${TOUCH} -t 200001010000 ${WRKSRC}/Makefile.util.def
	@${REINPLACE_CMD} -e '/[^auU]sed/ s,sed,gsed,g' ${WRKSRC}/util/grub.d/*.in \
		${WRKSRC}/util/*.in ${WRKSRC}/util/i386/efi/grub-dumpdevtree \
		${WRKSRC}/util/bash-completion.d/grub-completion.bash.in

pre-configure:
	cd ${WRKSRC} && ./bootstrap
	sed -i.bak  '23s/.*/typedef uint64_t u64;/' ${WRKSRC}/grub-core/lib/libgcrypt-grub/src/types.h

post-configure:
	@${LN} -sfh /usr/include/machine /usr/include/sys /usr/include/x86 \
		${WRKSRC}/grub-core

post-install:
.for f in boot_hybrid.img boot.img diskboot.img
	truncate -s 512 ${STAGEDIR}${PREFIX}/lib/grub/i386-pc/${f}
.endfor

.include <bsd.port.post.mk>
