#! /bin/sh

# http://stackoverflow.com/questions/59895/can-a-bash-script-tell-what-directory-its-stored-in
getScriptDir()
{
    local SOURCE="${BASH_SOURCE[0]}"
    local DIR=""

    # resolve $SOURCE until the file is no longer a symlink
    while [ -h "$SOURCE" ];
    do
        DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
        SOURCE="$(readlink "$SOURCE")"

        # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
        [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
    done

    DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

    echo $DIR
}

scriptDir=$(getScriptDir)

source "${scriptDir}/../env_base.sh" \
  "/path/to/synology/arm-marvell-linux-gnueabi" \
  "arm-marvell-linux-gnueabi" \
  "i686-pc-linux" \
  "/path/to/prefix"


