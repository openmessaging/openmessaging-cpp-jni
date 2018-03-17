#!/usr/bin/env bash

function clean() {
    if test -d dist; then
        rm -fr dist
    fi

    mkdir dist
}

function copy_headers() {
    cp -r include dist/
}

function copy_shared_libraries() {
    cp -r build/lib/* dist/
}

function copy_examples() {
    cp -r example dist/
    rm -f dist/example/CMakeLists.txt
}

function copy_vm() {
    cp -r third_party/jdk8m dist/vm
    rm -fr dist/vm/include
}

function copy_vendors() {
    mkdir -p dist/vendors/rocketmq
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



