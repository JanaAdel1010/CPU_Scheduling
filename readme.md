# CPU Scheduling Simulator

This project is a CPU Scheduling Simulator implemented in C++. It simulates various CPU scheduling algorithms using input files to define processes and outputs the results to files for validation and testing.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Makefile Targets](#makefile-targets)
- [Contributing](#contributing)

## Overview
The project is designed to simulate CPU scheduling algorithms by reading input data from text files, processing it according to the algorithm, and saving the output to files. The results are compared to expected outputs to verify correctness.

## Features
- Support for multiple input files.
- Automatic generation of output files.
- Comparison of generated output with expected output for correctness.
- Flexible and customizable with a `Makefile`
- a copy of the output is saved to the same directory as the testcases 



## Prerequisites
To run this project, you need:
- A C++ compiler that supports C++11 (e.g., `g++`).
- A Unix-like environment (Linux or macOS recommended).
- `make` utility installed.

## Usage
### 1. Prepare Input Files
Place all input files in the `testcases` directory. Each input file should define the processes for scheduling in the required format.

### 2. Build and run the Project
Run the following command to compile and run the project:
```bash
make
```

This will:
- Process all `.txt` files in the `inputs` directory.
- Save the results in the `outputs` directory with the suffix `-test` appended to the file name.
- Compare the output to the corresponding expected output file in `outputs`.

### 4. Clean Up
To clean up compiled files, run:
```bash
make clean
```

## Project Structure
```
CPU_Sch/
├── inputs/         # Input files for the simulator.
├── outputs/        # Generated output files.
├── algorithm.cpp   # Implementation scheduling algorithms.
├── print.cpp       # Utility functions for printing results.
├── process.cpp     # Process management functions.
├── main.cpp        # Main entry point of the program.
├── Makefile        # Build and run instructions.
└── README.md       # Project documentation.
```

## Makefile Targets
- **`all`**: Compiles the project.
- **`build`**: Same as `all`.
- **`run`**: Executes the simulator for all input files and saves results to `outputs`.
- **`clean`**: Removes compiled binaries and temporary files.



