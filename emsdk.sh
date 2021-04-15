#!/bin/bash

rm -rf build_emsdk
mkdir -p build_emsdk
cd build_emsdk
cmake .. -DCMAKE_TOOLCHAIN_FILE="../cmake/emscripten.cmake"
cmake --build . --config Release

#python3 -m http.server 8000 
