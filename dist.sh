#!/usr/bin/env bash

function clean() {
    if test -d dist; then
        rm -fr dist
    fi

    if test -f dist.tar.gz; then
        rm -f dist.tar.gz
        echo "Previous artifact dist.tar.gz is removed"
    fi

    mkdir dist
}

function copy_headers() {
    echo "Copying header files"
    cp -r include dist/
    echo "Done"
}

function copy_shared_libraries() {
    echo "Copying shared libraries"
    cp -r build/lib/* dist/
    echo "Done"
}

function copy_examples() {
    echo "Copying examples"
    cp -r example dist/
    rm -f dist/example/CMakeLists.txt
    echo "Done"
}

function copy_vm() {
    echo "Copying VirtualMachine"
    cp -r third_party/jdk8m dist/vm
    rm -fr dist/vm/include
    echo "Done"
}

function copy_vendors() {
    echo "Copying OMS vendor implementations"
    mkdir -p dist/vendors/rocketmq
    echo "Done"
}

function copy_artifacts() {
    copy_headers
    copy_shared_libraries
    copy_examples
    copy_vm
    copy_vendors
}

function build() {
    if test ! -d build; then
        mkdir build
    fi

    rm -fr build/*
    cd build
    cmake ..
    make
    cd ..
}

function execute_tests() {
    cd build
    ./runUnitTests
}

function main() {
    build
    clean
    copy_artifacts
    tar -czvf dist.tar.gz dist
}

main



