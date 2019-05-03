#!/bin/sh

# TrueOS Build Manifest
TM=${1}

# Where to create repo
STAGEDIR="${2}"

REPONAME="TrueOS"
if [ "$(jq -r '."pkg-repo-name" | length' ${TM})" != "0" ]; then
	REPONAME=`jq -r '."pkg-repo-name"' ${TM}`
fi
TRAINNAME="TrueOS"
if [ "$(jq -r '."pkg-train-name" | length' ${TM})" != "0" ]; then
	TRAINNAME=`jq -r '."pkg-train-name"' ${TM}`
fi
# Do the first-time setup of package repo
if [ ! -e "${STAGEDIR}/etc/pkg/Train.conf" ] ; then

	# Disable the FreeBSD repo by default
	if [ -e "${STAGEDIR}/etc/pkg/FreeBSD.conf" ] ; then
		mkdir -p ${STAGEDIR}/usr/local/etc/pkg/repos || true
		echo "FreeBSD: { enabled: no }" >${STAGEDIR}/usr/local/etc/pkg/repos/FreeBSD.conf
	fi

	# Do Setup for pkg repo
	TM_PUBKEY="none"
	if [ "$(jq -r '."pkg-repo"."pubKey" | length' ${TM})" != "0" ]; then
		echo "Saving pkg ports repository public key"
		jq -r '."pkg-repo"."pubKey" | join("\n")' ${TM} \
		> ${STAGEDIR}/usr/share/keys/pkg/${REPONAME}.pub
		TM_PUBKEY="/usr/share/keys/pkg/${REPONAME}.pub"
		if [ "$(jq -r '."pkg-repo"."url" | length' ${TM})" != "0" ]; then
			TM_PKGURL="$(jq -r '."pkg-repo"."url"' ${TM})"
			cat >${STAGEDIR}/etc/pkg/Train.conf <<-EOF
			${REPONAME}: {
			  url: "${TM_PKGURL}",
			  signature_type: "pubkey",
			  pubkey: "${TM_PUBKEY}",
			}
			EOF
		fi
	else
		if [ "$(jq -r '."pkg-repo"."url" | length' ${TM})" != "0" ]; then
			TM_PKGURL="$(jq -r '."pkg-repo"."url"' ${TM})"
			cat >${STAGEDIR}/etc/pkg/Train.conf <<-EOF
			${REPONAME}: {
			  url: "${TM_PKGURL}",
			  signature_type: "none",
			}
			EOF
		fi
	fi
fi
