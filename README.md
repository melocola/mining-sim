# Mining Simulation

## Overview
This project simulates a mining operation using various classes to manage miners, stations, and metrics collection. It showcases object-oriented programming practices and efficient data management.

## Components
- **Miner**: Represents individual miners.
- **Station**: Acts as collection points for resources.
- **MetricsHandler**: Collects and reports simulation metrics.
- **StationManager**: Manages stations.
- **TickHandler**: Manages simulation time and events.

## Getting Started
1. Clone the repository.
2. Ensure C++17 support and CMake are installed.
3. Build the project:
```
mkdir build && cd build
cmake ..
cmake --build .
```
4. Run the simulation with two parameters (number of miners and stations):
```
./mining-sim <miners> <stations>
```

Example: `./mining-sim 5 3`

## Simulation Output
- Upon completion, granular data is saved in a JSON file in the execution directory.
- Output values are multipliers for assumed base values. For distance traveled, multiply the simulation value by 20 (assuming 20 miles as the base distance).

## Dependencies
- C++17
- CMake
