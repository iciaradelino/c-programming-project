# C Programming Final Project
C implementation of a blackhole simulator


# Black Hole Simulation in C

A simulation of particles orbiting and falling into a black hole in C.

## Features
- Gravitational physics using Newtonian equations  
- Multithreading via POSIX threads  
- Modular design (physics, simulation, utils, rendering)  
- Exports data to `output.csv` for visualization  

## Build & Run

for example :

mkdir build && cd build
cmake ..
make
./blackhole 200
