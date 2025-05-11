#!/usr/bin/env bash

# Run the application
if [ ! -d "build" ]; then
    bash build.sh
fi

./build/leet
