#!/bin/bash

test_file=matfHub/test/parser.test.cpp
src_file=matfHub/src/parser.cpp

echo "Compiling..."
g++ -std=c++17 ${test_file} ${src_file} -o test_parser.out
echo "Compile successful"

echo "Running test..."
./test_parser.out
echo "Testing finished..."

