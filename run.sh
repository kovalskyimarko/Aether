#!/bin/bash
set -e

cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make
./Aether
