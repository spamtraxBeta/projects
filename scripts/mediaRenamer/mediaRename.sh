#! /bin/bash

if [[ -z $1 ]]
then
	PARALLEL_EXECUTIONS=8
	find . -maxdepth 1 -iname '*.jpg' -print0 | xargs -0 -n 1 -P $PARALLEL_EXECUTIONS -r -I {} $0 {}
	find . -maxdepth 1 -iname '*.thm' -print0 | xargs -0 -n 1 -P $PARALLEL_EXECUTIONS -r -I {} $0 {}
	find . -maxdepth 1 -iname '*.mts' -print0 | xargs -0 -n 1 -P $PARALLEL_EXECUTIONS -r -I {} $0 {}
else
	file="$1"

	fileDir=$(dirname "$file");
	fileName=$(basename "$file")
	extension="${fileName##*.}"
	fileName="${fileName%.*}"
	
	extensionLC=$(echo "$extension" | tr '[:upper:]' '[:lower:]')
	
	case "$extensionLC" in
		jpg)
			# 2015:08:22 09:08:15
			dateTime=$(exiv2 -P t -g Exif.Image.DateTime  pr  "$file")

			# 2015-08-22 09_08_15
			dateTime=$(echo $dateTime | sed -r "s/([0-9]{4}):([0-9]{2}):([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})/\1-\2-\3 \4_\5_\6/")

			outName="$dateTime ($fileName)"

			mv "$file" "$outName.$extension"

			arwFile="$fileDir/$fileName.ARW";

			if [ -f "$arwFile" ]
			then
				mv "$arwFile" "$outName.ARW"    
			fi
		;;
		
		thm)
			mp4File="$fileDir/$fileName.MP4"


			# 2015:08:22 09:08:15
			dateTime=$(exiv2 -P t -g Exif.Image.DateTime  pr  "$thmFile")

			# 2015-08-22 09_08_15
			dateTime=$(echo $dateTime | sed -r "s/([0-9]{4}):([0-9]{2}):([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})/\1-\2-\3 \4_\5_\6/")

			outName="$dateTime ($fileName)"

			mv "$thmFile" "$outName.$extension"
			mv "$mp4File" "$outName.MP4"
		;;
		
		mts)
			newName="$(echo "$fileName" | sed -r 's/^([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})$/\1-\2-\3 \4_\5_\6/')";

			if [[ "$fileName" != "$newName" ]]
			then
				echo "$file -> $newName ($fileName).$extension"
				mv "$file" "$newName ($fileName).$extension"
			else
				echo "Ignored file: $file"
			fi
		;;
		
	esac
fi


