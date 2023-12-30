#!/bin/bash

sudo apt-get install -y liblapack-dev

sudo apt-get install -y libblas-dev

sudo apt-get install -y libboost-dev

sudo apt-get install -y libarmadillo-dev

sudo apt-get install -y gnuplot

if [ ! -d "./matfHub/dependencies" ]; then
    mkdir -p ./matfHub/dependencies
fi

git clone https://github.com/Waqar144/QSourceHighlite.git ./matfHub/dependencies


