#!/bin/sh
CONFIGS="$1"
PLIST="$2"
STAGEDIR="$3"

get_prop_line() {
	stat -f '%Sf' ${STAGEDIR}/${1} | grep -q "schg"
	if [ $? -eq 0 ] ; then
		stat -f '%Su,%Sg,%OMp%OLp,schg' ${STAGEDIR}/${1}
	else
		stat -f '%Su,%Sg,%OMp%OLp,' ${STAGEDIR}/${1}
	fi
}

if [ -e "${PLIST}.new" ] ; then
	rm ${PLIST}.new
fi
touch ${PLIST}.new

# Read through the temp plist and cleanup
while read pline
do

	# Check for directory first
	echo ${pline} | grep -q "^@dir"
	if [ $? -eq 0 ] ; then
		pdir=$(echo $pline | awk '{print $2}')
		props=$(get_prop_line ${pdir})
		echo "@dir(${props}) ${pdir}" >> ${PLIST}.new
		continue
	fi

	props=$(get_prop_line $pline)

	# Is this a config file?
	config=0
	while read cfg
	do
		echo ${pline} | grep -q "^${cfg}"
		if [ $? -eq 0 ] ; then
			# This is a config file
			config=1
			break
		fi
	done < $CONFIGS

	if [ $config -eq 1 ] ; then
		echo "@config(${props}) ${pline}" >> ${PLIST}.new
	else
		echo "@(${props}) ${pline}" >> ${PLIST}.new
	fi


done < ${PLIST}
mv ${PLIST}.new ${PLIST}

