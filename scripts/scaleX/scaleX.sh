#! /bin/bash

ratio=$1

scaledDir="scaled_${ratio}";

mkdir -p "${scaledDir}"

findPattern="*.jpg"

numFiles=$(find . -maxdepth 1 -iname "${findPattern}" | wc -l)

onFile()
{
    local file="$1"
    echo $file
    convert "${file}" -resize "${ratio}%" "${scaledDir}/${file}"
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

