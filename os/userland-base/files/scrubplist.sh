#!/bin/sh
CONFIGS="$1"
PLIST="$2"
STAGEDIR="$3"

get_prop_line() {
	# Use a single stat due to it being expensive
	local SOUT=$(stat -f '%Sf %Su,%Sg,%OMp%OLp' ${STAGEDIR}/${1})
	local flags=$(echo $SOUT | awk '{print $1}')
	local prop=$(echo $SOUT | awk '{print $2}')

	echo $flags | grep -q "schg"
	if [ $? -eq 0 ] ; then
		echo "${prop},schg"
	else
		echo "${prop}"
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
	grep -q -w "${pline}" ${CONFIGS} 2>/dev/null
	if [ $? -eq 0 ] ; then
		echo "@config(${props}) ${pline}" >> ${PLIST}.new
	else
		echo "@(${props}) ${pline}" >> ${PLIST}.new
	fi


done < ${PLIST}
mv ${PLIST}.new ${PLIST}

