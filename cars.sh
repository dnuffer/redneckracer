#!/bin/bash
set -x
set -e
topdir="/Volumes/Home/dan/Dropbox/RedneckRacerGraphics/Car Animations"
dest="/Volumes/Home/dan/dev/redneckracer/graphics/ginormous/Cars"
for color in "Black" "Blue" "Brown" "Green" "Red" "Yellow"; do
	for dir in "North" "DrivesLeft" "DrivesRight" "TurnsLeft" "TurnsRight"; do
#		echo "$color $dir"
#		srcdir="$topdir/$color Car/${color}Car${dir}"
#		curdir="${dest}/${color}${dir}"
#		mkdir "$curdir"
#		cd "${srcdir}"
#		size=$(convert *.png -trim -layers merge -format "%wx%h%X%Y" info:)
#		for file in *.png; do 
#			convert "$file" -crop $size +repage "${curdir}/${file}"
#		done
		#14%
		#convert "$x" -resize 50% $(basename "$x")
		for file in $dest/$color$dir/*; do
			convert $file -resize 14% /Volumes/Home/dan/dev/redneckracer/graphics/wvga/$(basename $file)
			echo $file >> /Volumes/Home/dan/dev/redneckracer/graphics/wvga/race.atl
		done
	done
done
