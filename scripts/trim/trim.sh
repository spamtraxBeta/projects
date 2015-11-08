#! /bin/bash

fuzzPercentage=25
numCores=$(nproc)

printUsage()
{
	echo "Searches jpg files in current directory and passes them"
	echo "to convert from imagemagick to trim the files."
	echo ""
	echo "Usage: $(basename $0) [-f FuzzPercentage] [-j NumCores]"
	echo ""
	echo "Optional parameters: "
	echo "  -f: Fuzzing percentage for trim step;"
	echo "      Default value: $fuzzPercentage"
	echo ""
	echo "  -j: Number of parallel runs."
	echo "      Default value: $numCores"
	echo ""
	echo "Example calls:"
	echo "  $(basename $0)"
	echo "  $(basename $0) -f 42"
	echo "  $(basename $0) -j 3"
	echo "  $(basename $0) -f 42 -j 3"
	echo ""
	echo "Note: File names containing spaces are not supported at the moment."
}

# http://stackoverflow.com/questions/59895/can-a-bash-script-tell-what-directory-its-stored-in
getScriptDir()
{
	local SOURCE="${BASH_SOURCE[0]}"
	local DIR=""
	while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
	  SOURCE="$(readlink "$SOURCE")"
	  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
	done
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
	echo $DIR
}


while getopts ":f:j:h" opt; do
	case $opt in
		f)
			fuzzPercentage=$OPTARG;
		;;
		
		j)
			numCores=$OPTARG;
		;;
		
		h)
			printUsage
			exit 0
		;;
		
		\?)
			echo "Invalid option: -$OPTARG" >&2
			printUsage >&2
			exit 1
		;;
		
		:)
			echo "Missing argument for -$OPTARG" >&2
			printUsage >&2
			exit 1
		;;
	esac
done

scriptDir=$(getScriptDir)

# http://unix.stackexchange.com/questions/114943/can-sed-replace-new-line-characters
#make -f trim.mak  -j $numCores fuzzPercentage=$fuzzPercentage input="$(ls *.JPG *.jpg | sed  ':a;N;$!ba;s/\n/ /g')"
make -f trim.mak  -j $numCores fuzzPercentage=$fuzzPercentage input="$(find . -maxdepth 1 -iname "*.jpg" | sed  ':a;N;$!ba;s/\n/ /g')"



