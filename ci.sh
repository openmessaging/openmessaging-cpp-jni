#!/usr/bin/env bash

function build() {
    if test ! -d build; then
        mkdir build
    else
        rm -fr build/*
    fi

    cd build
    cmake ..
    make
    cd ..
}

function execute_tests() {
    cd build
    ./runUnitTests
}

build
execute_tests