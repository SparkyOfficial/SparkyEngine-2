#!/bin/bash

# Build script for Sparky Engine 2

echo "Building Sparky Engine 2..."
echo

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

# Navigate to build directory
cd build

# Generate build files with CMake
cmake ..

# Build the project
make

# Check if build was successful
if [ $? -eq 0 ]; then
    echo
    echo "Build successful!"
    echo
    echo "To run the game, execute:"
    echo "  cd Game"
    echo "  ./SparkyGame"
else
    echo
    echo "Build failed!"
fi