#!/bin/bash
set -x
set -e
set -u

size=$(convert *.png -trim -layers merge -format "%wx%h%X%Y" info:)

for frame in ./*.png; do
	convert "${frame}" -define png:color-type=6 -crop ${size} +repage "../${frame}"
done

