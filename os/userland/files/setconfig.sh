#!/bin/sh
CONFIGS="$1"
PLIST="$2"

while read cfg
do
	cat ${PLIST} | grep -v "^${cfg}" > $PLIST.new
	mv ${PLIST}.new ${PLIST}
	echo "@config $cfg" >> ${PLIST}
done < $CONFIGS
