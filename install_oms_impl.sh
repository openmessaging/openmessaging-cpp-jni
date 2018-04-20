#!/bin/bash
OMS_IMPL_DIR="${HOME}/oms"

if [ ! -d $OMS_IMPL_DIR/vendor ]; then
    mkdir -v -p $OMS_IMPL_DIR
    wget -O vendor.tar.gz http://shutian.oss-cn-hangzhou.aliyuncs.com/vendor.tar.gz
    tar -xzf vendor.tar.gz -C ${OMS_IMPL_DIR}
fi