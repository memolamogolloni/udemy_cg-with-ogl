#!/usr/bin/env bash
pushd .;
mkdir -p build && cd build;
rm -rf ./bin;
find . -iname "cmake*" -exec rm -rf {} \;
make && make run;
popd;