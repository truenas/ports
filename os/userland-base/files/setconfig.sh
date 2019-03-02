#!/bin/sh
CONFIGS="$1"
PLIST="$2"

while read cfg
do
	cat ${PLIST} | grep -q "^${cfg}"
	if [ $? -ne 0 ] ; then
		# This config wasn't found, skip adding to PLIST
		continue
	fi
	cat ${PLIST} | grep -v "^${cfg}" > $PLIST.new
	mv ${PLIST}.new ${PLIST}
	echo "@config $cfg" >> ${PLIST}
done < $CONFIGS
