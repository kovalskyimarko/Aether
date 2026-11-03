#!/bin/bash
set -e

rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make
./Aether
