#!/bin/bash

rm -rf build
mkdir build
mkdir -p TestingSystem/TestsResults

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUNROLL_LEVEL=2
cd build
make
./MandelbrotSet --test=50 --unroll > ../TestingSystem/TestsResults/Unroll2.txt
cd ..

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUNROLL_LEVEL=4
cd build
make
./MandelbrotSet --test=50 --unroll > ../TestingSystem/TestsResults/Unroll4.txt
cd ..

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUNROLL_LEVEL=8
cd build
make
./MandelbrotSet --test=50 --unroll > ../TestingSystem/TestsResults/Unroll8.txt
cd ..

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUNROLL_LEVEL=16
cd build
make
./MandelbrotSet --test=50 --unroll > ../TestingSystem/TestsResults/Unroll16.txt
cd ..

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUNROLL_LEVEL=32
cd build
make
./MandelbrotSet --test=50 --unroll > ../TestingSystem/TestsResults/Unroll32.txt

./MandelbrotSet --test=50 --native > ../TestingSystem/TestsResults/Native.txt

./MandelbrotSet --test=50 --simd   > ../TestingSystem/TestsResults/SIMD.txt

cd ..
cd TestingSystem
cd Plot
python3 -m venv venv
source venv/bin/activate
pip install matplotlib
pip install numpy
mkdir -p img
python PlotBuilder.py
deactivate

open img/*
