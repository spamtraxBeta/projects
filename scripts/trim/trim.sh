#! /bin/bash


printUsage()
{
	echo "Usage: "
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

fuzzPercentage=25
numCores=$(nproc)

while getopts ":f:j:" opt; do
	case $opt in
		f)
			fuzzPercentage=$OPTARG;
		;;
		
		j)
			numCores=$OPTARG;
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

make -f "$scriptDir/trim.mak" -j $numCores


