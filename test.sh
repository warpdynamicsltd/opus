#!/bin/bash 
cmake --build build --config Debug --target all -j 8 || exit
build/tests/tests