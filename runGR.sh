#!/bin/sh

mkdir Output
g++ -std=c++11 solver_bab.cpp -o run

echo "Running GRID"
mkdir Output/GRID
for i in `seq 1 35`;
do
    time ./run instanciasPRPP/GRID/G${i}NoRPP > outputG${i}.txt
    mv outputG${i}.txt Output/GRID
done

echo "Running RANDOM"
mkdir Output/RANDOM
for i in `seq 1 19`;
do
    time ./run instanciasPRPP/RANDOM/R${i}NoRPP > outputR${i}.txt
    mv outputR${i}.txt Output/RANDOM
done
