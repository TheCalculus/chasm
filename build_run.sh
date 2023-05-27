#!/bin/bash

# cd into build directory
cd build

# build using cmake
cmake .
# cmake --build

# make
make

# run executable
./chasm

# cd back to project root
cd ..
