#!/bin/bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DCMAKE_C_COMPILER=/usr/bin/gcc
cmake --build build --config Debug --target all -j 8 || exit
build/tests/tests