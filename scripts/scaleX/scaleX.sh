#! /bin/bash

ratio=$1
file=$2

PARALLEL=$(nproc)

if [[ -z $file ]]
then
	find . -maxdepth 1 -iname '*.JPG' -print0 | xargs -0 -n 1 -P $PARALLEL -I {} "$0" "$ratio" "{}"
else

	scaledDir="scaled_${ratio}";

	mkdir -p "${scaledDir}"

	command="convert '${file}' -resize '${ratio}%' '${scaledDir}/${file}'"

	echo $command
	eval "$command"
fi















exit 0

ratio=$1

scaledDir="scaled_${ratio}";

mkdir -p "${scaledDir}"

findPattern="*.jpg"

numFiles=$(find . -maxdepth 1 -iname "${findPattern}" | wc -l)

onFile()
{
    local file="$1"
    echo $file
    command="convert '${file}' -resize '${ratio}%' '${scaledDir}/${file}'"
	echo $command
	eval "$command"
}

export -f onFile
export numFiles
export scaledDir
export ratio

find . -maxdepth 1 -iname "${findPattern}" -exec bash -c 'onFile "$0"' {} \;





#for fullfile in *.JPG
#do
#
#    #mv "$fullfile" "$fullfile".big.jpg
#    #convert "$fullfile".big.jpg -resize 50% "$fullfile"
#    
#    convert "${fullfile}" -resize "${ratio}%" "${scaledDir}/${fullfile}"
#
#done

