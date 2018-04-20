#!/bin/bash
wget -O ${HOME}/boost_1_67_0.tar.gz https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz
tar -xzvf ${HOME}/boost_1_67_0.tar.gz -C ${HOME}
cd ${HOME}/boost_1_67_0
./bootstrap.sh --prefix=/usr/local --with-libraries=system,filesystem,chrono,thread,date_time
./bjam cflags="-fPIC" cxxflags="-fPIC" link=static --with-system --with-thread --with-date_time --with-filesystem --with-chrono  -a install