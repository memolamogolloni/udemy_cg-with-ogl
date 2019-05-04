#!/usr/bin/env bash

pushd . > /dev/null 2>&1
mkdir -p build && cd build && rm -rf ./bin > /dev/null 2>&1 &&
find . -iname "cmake*" -exec rm -rf {} \; > /dev/null 2>&1;
CC=gcc CXX=g++ make && make run
popd > /dev/null 2>&1