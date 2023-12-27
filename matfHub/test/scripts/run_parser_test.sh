#!/bin/bash

test_file=parser.test.cpp
src_file=../src/parser.cpp

echo "Compiling..."
g++ -std=c++17 ${test_file} ${src_file} -o test_parser.out
echo "Compile successful"

echo "Running test..."
./test_parser
echo "Testing finished..."

