#!/bin/bash 
cmake --build build --config Debug --target all -j 8
valgrind --leak-check=full build/tests/tests