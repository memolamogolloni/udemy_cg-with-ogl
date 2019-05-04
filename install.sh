#!/usr/bin/env bash

pushd . > /dev/null 2>&1
rm -rf build > /dev/null 2>&1
mkdir -p build && cd build && conan install .. && CC=gcc CXX=g++ cmake ..
popd > /dev/null 2>&1