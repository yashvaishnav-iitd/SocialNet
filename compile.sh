#!/bin/bash
# This script compiles the socialnet.cpp file into an executable named 'socialnet'
# It uses the g++ compiler with the C++11 standard.

echo "Compiling main.cpp..."
g++ main.cpp -o socialnet -std=c++11 -Wall
echo "Compilation finished. Run with ./socialnet"
