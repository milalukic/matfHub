#!/bin/bash

sudo apt-get install -y liblapack-dev

sudo apt-get install -y libblas-dev

sudo apt-get install -y libboost-dev

sudo apt-get install -y libarmadillo-dev

sudo apt-get install -y gnuplot

if [ ! -d "./matfHub/dependencies" ]; then
    mkdir -p ./matfHub/dependencies
fi

DEPENDENCIES_DIR="./matfHub/dependencies"

if [[ ! -e ${DEPENDENCIES_DIR}/.git ]]; then
    # Directory is not a Git repository and is not empty, delete everything
    rm -rf ${DEPENDENCIES_DIR}
    mkdir ${DEPENDENCIES_DIR}

    # Perform the clone
    git clone https://github.com/Waqar144/QSourceHighlite.git "$DEPENDENCIES_DIR"
    cd ${DEPENDENCIES_DIR}
    rm -rf .git
    rm main.cpp
    rm mainwindow.cpp
    rm mainwindow.h
    rm mainwindow.ui
    
fi
