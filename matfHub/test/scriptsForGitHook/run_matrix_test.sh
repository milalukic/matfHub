#!/bin/bash
set -e

cd matfhub/test/scriptForGitHook

test_file=matrix.test.cpp
src_file=../../src/matrix.cpp

echo "Compiling..."
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$HOME/Qt/6.6.0/gcc_64/lib/pkgconfig
#g++ -std=c++17 $(pkg-config --cflags --libs Qt6Widgets) -I$HOME/Qt/6.6.0/gcc_64/include -I$HOME/Qt/6.6.0/gcc_64/include/QtCore -I$HOME/Qt/6.6.0/gcc_64/include/QtWidgets ${test_file} ${src_file} -o test_fmanager.out -fPIC -lQt6Widgets -lQt6Core
#g++ -std=c++17 $(pkg-config --cflags --libs Qt6Widgets) -I/usr/include/qt/ -I/usr/include/qt/QtCore -I/usr/include/qt/QtWidgets ${test_file} ${src_file} -o test_fmanager.out -fPIC -lQt6Widgets -lQt6Core
g++ -std=c++17 $(pkg-config --cflags --libs Qt6Widgets) -I$HOME/Qt/6.6.0/gcc_64/include -I$HOME/Qt/6.6.0/gcc_64/include/QtCore -I$HOME/Qt/6.6.0/gcc_64/include/QtWidgets ${test_file} ${src_file} -o test_matrix.out -fPIC -L$HOME/Qt/6.6.0/gcc_64/lib -lQt6Widgets -lQt6Core -larmadillo
echo "Compile successful"

echo "Running test..."
export LD_LIBRARY_PATH=$HOME/Qt/6.6.0/gcc_64/lib

./test_matrix.out
cd ../../..
echo "Testing finished..."
