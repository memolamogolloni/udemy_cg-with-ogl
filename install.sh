#!/usr/bin/env bash
pushd .;
rm -rf build;
mkdir -p build && cd build;
conan install .. && cmake ..;
popd;