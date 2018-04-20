#!/bin/bash
if [ ! -d /usr/local/lib/oms/vendor ]; then
    mkdir -p /usr/local/lib/oms
    wget -O vendor.tar.gz http://shutian.oss-cn-hangzhou.aliyuncs.com/vendor.tar.gz
    tar -xzf vendor.tar.gz -C /usr/local/lib/oms
fi