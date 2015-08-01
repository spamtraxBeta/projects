#! /bin/bash

# "source" this script to setup a cross compile environment for raspberry pi


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
  "/path/to/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64" \
  "arm-linux-gnueabihf" \
  "i686-pc-linux" \
  "/path/to/prefixDir"

vcDir="/path/to/raspi/firmware/opt/vc"
export CFLAGS="$CFLAGS -I${vcDir}/include"
export LDFLAGS="$LDFLAGS -L${vcDir}/lib"

