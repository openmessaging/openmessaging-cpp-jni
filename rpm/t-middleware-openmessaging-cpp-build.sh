#!/bin/bash

cd $1

WORK_DIR=`pwd`

BOOST=boost_1_67_0

if [ ! -d ${WORK_DIR}/third_party/${BOOST} ]; then
    wget -O ${WORK_DIR}/third_party/boost_1_67_0.tar.gz https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz
    tar -xzf ${WORK_DIR}/third_party/boost_1_67_0.tar.gz -C ${WORK_DIR}/third_party
fi

cd ${WORK_DIR}/third_party/${BOOST}

./bootstrap.sh --prefix=/usr/local --with-libraries=system,filesystem,chrono,thread,date_time

./bjam cflags="-fPIC" cxxflags="-fPIC" link=static \
       --with-system --with-thread --with-date_time --with-filesystem --with-chrono  \
       -a install

cd $1/rpm
rpm_create -v $3 -r $4 $2.spec -p /home/admin/$2