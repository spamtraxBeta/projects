#! /bin/bash

# This script searches JPG and THM files in current directory.
# Extracts EXIF data and renames JPG files from "xxx.JPG" to "YYYY-MM-DD HH:MM:SS (xxx).JPG".
# Same pattern is applied to THM files and additionally to MP4 files with the same name as the
# THM file.


onJpgFile()
{
    local file="$1"
    
    fileDir=$(dirname "$file");
    fileName=$(basename "$file")
    extension="${fileName##*.}"
    fileName="${fileName%.*}"
    
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
    
    
#    local file="$1"
#
#    echo "renaming $file..."
#    exiv2 -r "%Y-%m-%d %H_%M_%S (:basename:)" mv "$file"
#    #exiftool -orientation= *.JPG
}

onThmFile()
{
    local thmFile="$1";
    
    fileDir=$(dirname "$thmFile");
    fileName=$(basename "$thmFile")
    extension="${fileName##*.}"
    fileName="${fileName%.*}"
    
    mp4File="$fileDir/$fileName.MP4"
    
    
    # 2015:08:22 09:08:15
    dateTime=$(exiv2 -P t -g Exif.Image.DateTime  pr  "$thmFile")
    
    # 2015-08-22 09_08_15
    dateTime=$(echo $dateTime | sed -r "s/([0-9]{4}):([0-9]{2}):([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})/\1-\2-\3 \4_\5_\6/")
    
    outName="$dateTime ($fileName)"
    
    mv "$thmFile" "$outName.$extension"
    mv "$mp4File" "$outName.MP4"
}


export -f onJpgFile
export -f onThmFile

find . -maxdepth 1 -iname "*.jpg" -exec bash -c 'onJpgFile "$0"' {} \;
find . -maxdepth 1  -iname "*.thm" -exec bash -c 'onThmFile "$0"' {} \;


##exiv2 -r "%Y-%m-%d %H_%M_%S (:basename:)" mv *.jpg
#! /bin/bash

# This script searches JPG and THM files in current directory.
# Extracts EXIF data and renames JPG files from "xxx.JPG" to "YYYY-MM-DD HH:MM:SS (xxx).JPG".
# Same pattern is applied to THM files and additionally to MP4 files with the same name as the
# THM file.



onJpgFile()
{
    local file="$1"

    echo "renaming $file..."
    exiv2 -r "%Y-%m-%d %H_%M_%S (:basename:)" mv "$file"
    #exiftool -orientation= *.JPG
}

onThmFile()
{
    local thmFile="$1";
    
    fileDir=$(dirname "$thmFile");
    fileName=$(basename "$thmFile")
    extension="${fileName##*.}"
    fileName="${fileName%.*}"
    
    mp4File="$fileDir/$fileName.MP4"
    
    
    # 2015:08:22 09:08:15
    dateTime=$(exiv2 -P t -g Exif.Image.DateTime  pr  "$thmFile")
    
    # 2015-08-22 09_08_15
    dateTime=$(echo $dateTime | sed -r "s/([0-9]{4}):([0-9]{2}):([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})/\1-\2-\3 \4_\5_\6/")
    
    outName="$dateTime ($fileName)"
    
    mv "$thmFile" "$outName.$extension"
    mv "$mp4File" "$outName.MP4"
}

export -f onJpgFile
export -f onThmFile

find . -maxdepth 1 -iname "*.jpg" -exec bash -c 'onJpgFile "$0"' {} \;
find . -maxdepth 1  -iname "*.thm" -exec bash -c 'onThmFile "$0"' {} \;


##exiv2 -r "%Y-%m-%d %H_%M_%S (:basename:)" mv *.jpg
