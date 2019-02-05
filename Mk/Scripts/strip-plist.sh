#!/bin/sh

PLIST="$1"
STRIPLIST="$2"

if [ ! -e "$STRIPLIST" ] ; then
	echo "ERROR: Missing $STRIPLIST" >&2
	exit 1
fi

# If the user has specified a list of files / expressions to strip from
# a ports plist, lets handle that now
while read strip
do
	grep -q " $strip" ${PLIST}
	if [ $? -ne 0 ] ; then continue ; fi

	cat ${PLIST} | grep -v " $strip" > ${PLIST}.new
	mv ${PLIST}.new ${PLIST}
done < ${STRIPLIST}

