#!/bin/bash
OMS_IMPL_DIR="/usr/local/lib/oms"

if [ ! -d $OMS_IMPL_DIR/vendor ]; then
    mkdir -p $OMS_IMPL_DIR

    if [ ! -d $OMS_IMPL_DIR ]; then
        echo "Create directory $OMS_IMPL_DIR failed"
    fi

    wget -O vendor.tar.gz http://shutian.oss-cn-hangzhou.aliyuncs.com/vendor.tar.gz
    tar -xzf vendor.tar.gz -C /usr/local/lib/oms
fi