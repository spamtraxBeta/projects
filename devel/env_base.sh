#! /bin/sh

# This script sets up a cross compile environment

# "source" it with following parameters:


# examples:
# /path/to/arm-marvell-linux-gnueabi
# /path/to/raspi/toolchain/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64
toolPath="$1"

# examples:
#  arm-marvell-linux-gnueabi
#  arm-linux-gnueabihf
target="$2"

# example: i686-pc-linux
host="$3"

# example: "/path/to/prefixDir"
prefix="$4"

############################################################
# set environment variables
############################################################
cross_include="${toolPath}/include"
cross_lib="${toolPath}/lib"

export CROSS_COMPILE=${toolPath}/bin/${target}-
export CC=${toolPath}/bin/${target}-gcc 
export LD=${toolPath}/bin/${target}-ld 
export RANLIB=${toolPath}/bin/${target}-ranlib
export CFLAGS="-I${cross_include} -I${prefix}/include"
export LDFLAGS="-L${cross_lib} -L${prefix}/lib"

export cc_host=${target}
export cc_target=${target}
export cc_build=${host}
export cc_prefix=${prefix}

export PATH=$PATH:"${toolPath}/bin"
