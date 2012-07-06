#!/bin/bash

# this script depends on resize_fonts.sh, resize_assets.sh and generate_atlases.sh

# (NOTE: generate_atlases.sh depends on mkatlas.pl which depends on compression tools: etc1tool and texturetool)

# for simplicity, this script can be run with a single parameter for a target device (android or iphone).
# Or it can be run with custom size, compression and directory options. See usage for details.

set -u #referencing undefined variable causes error
set -x #print out everything it does
set -e #anything with non-zero exit status aborts script

usage()
{
	echo "USAGE: Option 1: <target-device (android or iphone)>"
	echo "       Option 2: <target-width> <target height> <compression (etc1, pvr or none)> [<graphics-src-dir> <resized-dest-dir> <atlas-dest-dir>]" 
	exit
}

checkCompression()
{
	if [[ ${1} != 'pvr' && ${1} != 'etc1' && ${1} != 'none' ]]; then
		echo "error: unsupported compression specification: ${1}"
		usage
	fi 
}

checkSrcDirectory()
{
	if [ ! -d "${1}" ]; then
		echo "specified source directory does not exist: ${1}"
		exit
	fi
}

#defaults
compression='none'
graphicsSrcDir='./graphics/ginormous'
resizedGraphicsDir='./graphics/wvga'
atlasesDir='./assets'

targetDevice='android'
targetW=480
targetH=800

if [ $# == 6 ]; then
	targetW=${1}
	targetH=${2}
	compression="${3}"
	graphicsSrcDir="${4}"
	resizedGraphicsDir="${5}"
	atlasesDir="${6}"
elif [ $# == 5 ]; then
	targetW=${1}
	targetH=${2}
	compression="${3}"
	graphicsSrcDir="${4}"
	resizedGraphicsDir="${5}"
elif [ $# == 3 ]; then
	targetW=${1}
	targetH=${2}
	compression=${3}
elif [ $# == 2 ]; then
	targetW=${1}
	targetH=${2}
elif [ $# == 1 ]; then
	
	targetDevice=${1}
	
	if [ "${targetDevice}" == 'android' ]; then
		compression='etc1'
		targetW=480
		targetH=800
	elif [ "${targetDevice}" == 'iphone' ]; then
		compression='pvr'
		targetW=640
		targetH=960
	else
		echo "error: unsupported target device: ${targetDevice}"
		usage
	fi
else
	echo "error: unable to interpret parameters"
	usage
fi

checkCompression "${compression}"
checkSrcDirectory "${graphicsSrcDir}"

#add android sdk tools dir (for etc1tool) to the PATH variable if the directory exists
if [ -e local.properties ]; then
	sdkpath=$(grep 'sdk\.dir' local.properties | cut -s -d= -f2)
	#FIXME! check if sdkpath is assigned or not	
	PATH="${PATH}:${sdkpath}/tools"
fi

echo "target screen resolution: ${targetW}x${targetH}"
echo "compression: ${compression}"
echo "full-size graphics src dir: ${graphicsSrcDir}"
echo "resized graphics dest dir: ${resizedGraphicsDir}"
echo "atlas dest dir: ${atlasesDir}"

# clear out atlas dir	
find ${atlasesDir} -type f | egrep -v '(sounds/|fonts/)' | xargs rm -f

./resize_assets.sh  ${targetW} ${targetH} "${graphicsSrcDir}" "${resizedGraphicsDir}" || { echo "FAILED!"; exit 1; }

mv "${resizedGraphicsDir}/"*".road" "${resizedGraphicsDir}/realrect.bound" "${atlasesDir}"

./generate_atlases.sh ${compression} "${resizedGraphicsDir}" "${atlasesDir}" || { echo "FAILED!"; exit 1; }
./resize_fonts.sh ${targetH} "${graphicsSrcDir}" "${atlasesDir}/fonts" || { echo "FAILED!"; exit 1; }

echo "Finished!"
