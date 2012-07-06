#!/bin/bash

# generates a resized font image with scaled .atlas file according to a target screen height parameter

# depends on a font.resize.data file in the following format:
# font=<sourcefontname>:outputname=<targetfontname>:ratio=<float value-letterheight-to-screenheight-ratio>:height=<sourceletterheight>
# the outputname parameter allows for multiple sizes of a font to be generated from a single source font image.
# the ratio parameter should be set according to how large the font should appear relative to the screen height

# depends on a <fontname>.png font with an associated <fontname>.atlas file in the following format:
# note: extra spacing is used between letters in the generated font-image in order to preserve adequate spacing (approx. 4 pixels)
# after maximum resize.

# image: fonts/TheMilkmanConspiracy.png
# size: 2048 2048
# group: TheMilkmanConspiracy
# quad: fonts/TheMilkmanConspiracy/032 50 50 40 129 1
# quad: fonts/TheMilkmanConspiracy/033 140 50 40 129 1
# quad: fonts/TheMilkmanConspiracy/034 230 50 51 129 1
# ...

# <fontname>.atlas body line format: 
# "quad: dir/name/letterNum xLetterCoord yLetterCoord letterWidth letterHeight"

set -u #referencing undefined variable causes error
set -x #print out everything it does
set -e #anything with non-zero exit status aborts script

usage()
{
	echo "USAGE: <target-Screen-Height> <font-image-src-dir> <font-image-dest-dir>"
	exit 1
}

main()
{
	if [ $# -ne 3 ]
	then
		usage
	fi
	
	ending='.png'
	targetScreenHeight=${1}
	srcDir="${2}"
	destDir="${3}"

	mkdir -p "${destDir}"

	find "${srcDir}" -type f -name font.resize.data | while read fontResizeFile; do
	
		resizeDir=$(dirname "${fontResizeFile}")
		
		cat ${fontResizeFile} | while read fontResizeLine; do	
			
			fontLineWithNewLineDelim=$(echo ${fontResizeLine} | tr ':' '\n')
				
			echo "${fontLineWithNewLineDelim}" > ${resizeDir}/resize.tmp

			sourceFontName=$(grep 'font=' "${resizeDir}/resize.tmp"  | cut -s -d= -f2)
			targetFontName=$(grep 'outputname=' "${resizeDir}/resize.tmp"  | cut -s -d= -f2)
			letterToScreenHeightRatio=$(grep 'ratio=' "${resizeDir}/resize.tmp"  | cut -s -d= -f2)
			ginormousLetterHeight=$(grep 'height=' "${resizeDir}/resize.tmp"  | cut -s -d= -f2)
			
			fontImage=$(echo "${sourceFontName}${ending}")
			ginormousFontWidth=$(convert "${resizeDir}/${fontImage}" -format %w info:)
			
			rm "${resizeDir}/resize.tmp"

			targetLetterHeight=$(echo "scale=12; ${targetScreenHeight} * ${letterToScreenHeightRatio}" | bc)
			fontScale=$(echo "scale=12; ${targetLetterHeight} / ${ginormousLetterHeight}" | bc)
			targetFontWidth=$(echo "scale=12; ${ginormousFontWidth} * ${fontScale}" | bc)

			convert ${resizeDir}/${fontImage} -define png:color-type=6 -resize ${targetFontWidth} ${destDir}/resized${fontImage}

			#font atlas needs to be square
			#calculate next highest power of two
			powOf2=1

			while [ $(echo "scale=1; ${powOf2} < ${targetFontWidth}" | bc) -ne 0 ]; do 
				powOf2=$(echo "scale=0; ${powOf2} * 2" | bc)
			done
			
			#pad image on bottom and right with transparent pixels to the above calculated next highest power of 2
			convert ${destDir}/resized${fontImage} -background transparent -gravity NorthWest -extent ${powOf2}x${powOf2} -define png:color-type=6 ${destDir}/${fontImage}

			rm -f ${destDir}/resized${fontImage}

			#scaling font atlas file
			
			#Line 1: copy straight over - image: fonts/TheMilkmanConspiracy.png
			image=$(grep 'image:' "${resizeDir}/${sourceFontName}.atlas" | cut -s -d' ' -f2)
			#Line 3: copy straight over - group: TheMilkmanConspiracy
			group=$(grep 'group:' "${resizeDir}/${sourceFontName}.atlas" | cut -s -d' ' -f2)
			
			#font atlas header
			echo image: ${image} > ${destDir}/${sourceFontName}.atlas
			echo size: ${powOf2} ${powOf2} >> ${destDir}/${sourceFontName}.atlas
			echo group: ${group} >> ${destDir}/${sourceFontName}.atlas
			
			#font atlas body line format: quad: dir/name/letterNum xLetterBound yLetterBound letterWidth letterHeight
			#example - quad: fonts/TheMilkmanConspiracy/032 4 4 40 129 1
			
			grep 'quad:' ${resizeDir}/${sourceFontName}.atlas | while read quadLine; do
			
				letterID=$(echo "${quadLine}" | cut -s -d' ' -f2)
				xLetterBound=$(echo "${quadLine}" | cut -s -d' ' -f3) 
				yLetterBound=$(echo "${quadLine}" | cut -s -d' ' -f4) 
				letterWidth=$(echo "${quadLine}" | cut -s -d' ' -f5) 
				letterHeight=$(echo "${quadLine}" | cut -s -d' ' -f6)
				
				#scale and round letter bounds to nearest whole number
				newXLetterBound=$(echo "scale=12; ${xLetterBound} * ${fontScale}" | bc | xargs printf "%1.0f") 
				newYLetterBound=$(echo "scale=12; ${yLetterBound} * ${fontScale}" | bc | xargs printf "%1.0f")
				newLetterWidth=$(echo "scale=12; ${letterWidth} * ${fontScale}" | bc | xargs printf "%1.0f")
				newLetterHeight=$(echo "scale=12; ${letterHeight} * ${fontScale}" | bc | xargs printf "%1.0f")
				
				xScale=1
				yScale=1
				
				echo quad: "${letterID} ${newXLetterBound} ${newYLetterBound} ${newLetterWidth} ${newLetterHeight} ${xScale} ${yScale}" >> ${destDir}/${sourceFontName}.atlas
			done
			
			
		done
	done

}

main ${@}

