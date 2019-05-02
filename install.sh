#!/usr/bin/env bash

pushd .
rm -rf build > /dev/null 2>&1
mkdir -p build && cd build && conan install .. && cmake .. > /dev/null 2>&1
popd