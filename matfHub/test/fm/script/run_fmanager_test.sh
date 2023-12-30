#!/bin/bash

test_file=fm/fmanager.test.cpp
src_file=../src/filemanager.cpp

echo "Compiling..."

export PKG_CONFIG_PATH=$HOME/Qt/6.6.0/gcc_64/lib/pkgconfig:$PKG_CONFIG_PATH

#g++ -std=c++17 -I/$HOME/Qt/6.6.0/gcc_64/include -I/$HOME/Qt/6.6.0/gcc_64/include/QtWidgets $(pkg-config --cflags --libs Qt6Widgets) -fPIC ${test_file} ${src_file} -o test_fmanager.out

#g++ -std=c++17 -I$HOME/Qt/6.6.0/gcc_64/include $(pkg-config --cflags --libs Qt6Widgets) -fPIC ${test_file} ${src_file} -o test_fmanager.out

g++ -std=c++17 -I$HOME/Qt/6.6.0/gcc_64/include -I$HOME/Qt/6.6.0/gcc_64/include/QtCore -I$HOME/Qt/6.6.0/gcc_64/include/QtWidgets -I$HOME/Qt/6.6.0/gcc_64/include/Qt ${test_file} ${src_file} -o test_fmanager.out -fPIC -lQt6Widgets -lQt6Core

#g++ -std=c++17 $(pkg-config --cflags --libs Qt6Widgets) -fPIC ${test_file} ${src_file} -o test_fmanager.out

#g++ -std=c++17 -I$HOME/Qt/6.6.0/gcc_64/include -I$HOME/Qt/6.6.0/gcc_64/include/QtWidgets ${test_file} ${src_file} -o test_fmanager.out

echo "Compile successful"

echo "Running test..."
cd fm/script
./test_fmanager.out
echo "Testing finished..."

