#!/usr/bin/env bash

dir=`dirname $0`
cd $dir

if test -d build; then
    echo "To remove existing build directory"
    rm -fr build
    echo "Removing completes OK"
fi

WORK_DIR=`pwd`
mkdir build
cd build
cmake ..