#/bin/bash

if [ "$#" -nt 2 ]; then
    echo "Usage: $0 <device> <path_to_exe> <arguments>"
    return -1
fi

DEVICE=$1
shift
COMMAND=$1
shift

BIND2DEVICE=$DEVICE LD_PRELOAD=./bind2device.so $COMMAND "$@"
