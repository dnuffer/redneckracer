#!/bin/bash

set -u #referencing undefined variable causes error
#set -x #print out everything it does
#set -e #anything with non-zero exit status aborts script

resizeGraphics()
{ 
 find . -type f -name 'resize.data' | while read filename; do
    cat "$filename" | while read line
    do
		if [ x${line:+set} = xset ]; then
			 glob=$(echo "${line}" | cut -d: -f1)
			 atlas=$(echo "${line}" | cut -d: -f2)
			 #globSafe=$(echo ${glob} | sed 's/\(\*\)/?/')
			 #percentageLine=$(grep ${globSafe} precision_percentages.txt)
			 #resize=$(echo ${percentageLine} | cut -d: -f2)
			 resize=$(grep "${glob%?.png}" precision_percentages.txt | cut -d: -f2)
			 compression=$(echo "$line" | cut -d: -f4)
			 
			 #echo "${line}"
			 echo "glob=${glob}:atl=${atlas}:resize=${resize}" >> "${filename}".new
			 #rm -f "${filename}".new
			 

		fi
    done
    
	rm ${filename}	 
	mv ${filename}.new ${filename}
	
 done
}


main()
{
resizeGraphics $1
}

main "$@"
