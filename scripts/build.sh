#!/bin/bash

# Check if "build" directory exists
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
else
    echo "Build directory already exists."
fi

# Change to the build directory
cd build || exit

# Run cmake and make commands
cmake ..
make

