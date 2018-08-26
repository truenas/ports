#!/bin/sh
# Helper script to checkout FreeNAS ports to a specific
# Branch or tag for building
#

# List of ports to update
# Usage: <portname>::<project>::<repo>
PLIST="mDNSResponder::freenas::mDNSResponder"
#PLIST="${PLIST} freenas::freenas::freenas"

usage()
{
	echo "Updates FreeNAS Ports to a requested GitHub branch or tag"
	echo ""
	echo "Usage: $0 port branch"
	echo ""
	echo "Example: $0 mDNSResponder master"
	echo "Example: $0 all master"
	exit 1
}

update_port()
{
	local port="$(echo $1 | awk -F"::" '{print $1}')"
	local project="$(echo $1 | awk -F"::" '{print $2}')"
	local repo="$(echo $1 | awk -F"::" '{print $3}')"

	GH_HASH=$(fetch -o - https://api.github.com/repos/$project/$repo/git/refs/heads/$BRANCH 2>/dev/null | jq -r '."object"."sha"')
	GH_DATE=$(fetch -o - https://api.github.com/repos/$project/$repo/commits/$GH_HASH 2>/dev/null | jq -r '."commit"."author"."date"')

	#echo "$GH_HASH"
	#echo "$GH_DATE"

	YEAR="$(echo $GH_DATE | cut -d '-' -f 1)"
	MON="$(echo $GH_DATE | cut -d '-' -f 2)"
	DAY="$(echo $GH_DATE | cut -d '-' -f 3 | cut -d 'T' -f 1)"
	HOUR="$(echo $GH_DATE | cut -d 'T' -f 2 | cut -d ':' -f 1)"
	MIN="$(echo $GH_DATE | cut -d 'T' -f 2 | cut -d ':' -f 2)"
	SEC="$(echo $GH_DATE | cut -d 'T' -f 2 | cut -d ':' -f 3 | cut -d 'Z' -f 1)"
	TSTAMP="$YEAR$MON$DAY$HOUR$MIN$SEC"

	#echo "$TSTAMP"

	sed -i '' "s/.*PORTVERSION=.*/PORTVERSION=	$TSTAMP/" ${port}/Makefile
	sed -i '' "s/.*GH_TAGNAME=.*/GH_TAGNAME=	$GH_HASH/" ${port}/Makefile

	make -C ${port} makesum
}

if [ -z "$1" -o -z "$2" ] ; then
	usage
fi

BRANCH="$2"

if [ "$1" = "all" ] ; then
	for p in $PLIST
	do
		update_port $p
	done
else
	port=$(echo $PLIST | tr ' ' '\n' | grep "^${1}::")
	if [ -z "$port" ] ; then
		echo "ERROR: Invalid port $1 specified"
		exit 1
	fi
	update_port ${port}
fi
