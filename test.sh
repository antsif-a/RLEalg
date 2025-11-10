#!/bin/sh
mkdir -p build && cmake . -Bbuild && cmake --build build
cd build
make

./RLEalg code ../test/$1 ../test/$1.rle
./RLEalg decode ../test/$1.rle ../test/$1.rle.decode

cd ..
cmp test/$1 test/$1.rle.decode