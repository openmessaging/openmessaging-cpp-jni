#!/bin/bash

BOOST=boost_1_67_0

if [ ! -d ${HOME}/${BOOST} ]; then
    wget -O ${HOME}/boost_1_67_0.tar.gz https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz
    tar -xzf ${HOME}/boost_1_67_0.tar.gz -C ${HOME}
fi

cd ${HOME}/${BOOST}

./bootstrap.sh --prefix=/usr/local --with-libraries=system,filesystem,chrono,thread,date_time

./bjam cflags="-fPIC" cxxflags="-fPIC" link=static \
       --with-system --with-thread --with-date_time --with-filesystem --with-chrono  \
       -a install \
       > ${HOME}/build.log

if [ $? != 0 ]; then
    tail -n 300 ${HOME}/build.log
fi