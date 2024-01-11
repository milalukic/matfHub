#!/bin/bash

test -d build
build_check=$?

if [[ build_check -eq 1 ]]
then
    echo "build directory not found"
    echo "Making a build directory..."
    mkdir build
fi

echo "Switching to build directory"
cd build

echo "Running CMake..."
cmake -DENABLE_COVERAGE=true .. && make
echo "CMake successfully finished"

echo "Running generated Makefile..."
make
echo "Makefile successfully finished..."

echo "Starting test_stat"
echo "Program running..."
./test_plot && make coverage
echo "Testing finished...\n"
