#!/bin/bash
set -x
set -e
set -u

#generate resize.data for grassy background image set printed to ./resize.data file

prefix="$1"
absoluteWidth="$2"
absoluteHeight="$3"
#prefix=background_grassy
#absoluteWidth=508
#absoluteHeight=1020

for trackName in ./${prefix}*.png; do

	echo glob=$(basename ${trackName}):atl=$(basename ${trackName%.png}).atl:resize=${absoluteWidth}x${absoluteHeight}:compression=yes >> resize.data
	
done
