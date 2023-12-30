#!/bin/bash

apt-get install -y liblapack-dev

apt-get install -y libblas-dev

apt-get install -y libboost-dev

apt-get install -y libarmadillo-dev

apt-get install -y gnuplot

if [ ! -d "./matfHub/dependencies" ]; then
    mkdir -p ./matfHub/dependencies
fi

DEPENDENCIES_DIR="./matfHub/dependencies"

if [ ! -e "$DEPENDENCIES_DIR/.git" ] && [ -z "$(ls -A $DEPENDENCIES_DIR)" ]; then
    # Directory is not a Git repository and is not empty, delete everything
    rm -rf "$DEPENDENCIES_DIR"/*
    
    # Perform the clone
    git clone https://github.com/Waqar144/QSourceHighlite.git "$DEPENDENCIES_DIR"
else
    # Directory is either a Git repository or is empty, consider pulling the latest changes
    (cd "$DEPENDENCIES_DIR" && git pull)
fi


