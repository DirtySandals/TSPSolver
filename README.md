<h1>TSPSolver and TSPGUI</h1>
<h2>Overview</h2>
This repository contains two main folders:

- TSPGUI: A Python-based graphical user interface (GUI) for interacting with the TSPSolver.
- TSPSolver: A C++ implementation that uses a genetic algorithm to solve the Traveling Salesman Problem (TSP).

<h2>TSPSolver</h2>
<h3>Description:</h3>
The TSPSolver folder houses a C++ implementation that solves the TSP problem using a genetic algorithm. It accepts instance files and can be run via terminal/command line.

<h3>Key Features of TSPSolver:</h3>

- The TSPSolver employs a genetic algorithm find optimal solutions for the TSP
- Mutation Operators:
  - Insert Mutation
  - Swap Mutation
  - Inversion Mutation
  - Scramble Mutation
- Crossover Operators:
  - Order Crossover
  - PMX Crossover
  - Cycle Crossover
- Selection Operators:
  - Fitness Proportional Selection
  - Tournament Selection
  - Elitism Selection
- Inverover Algorithm: An additional algorithm from Guo Tao and Zbigniew Michalewicz for solving TSP

<h3>Running TSPSolver:</h3>
The solver can be run from the terminal by providing TSP instance files as input.
Alternatively, users can interact with TSPSolver through command-line arguments for various customization options.

<h2>TSPGUI</h2>
<h3>Description:</h3>
The TSPGUI folder contains a Python script built with Pygame that serves as a graphical user interface for the TSPSolver.

<h3>Key Features of TSPGUI:</h3>
User-Friendly Interface: Allows users to interact visually with the TSP instance files and the TSPSolver.
Real-Time Interaction: Provides real-time interaction with the genetic algorithm and visual feedback on the TSP solution process.
Visual Display: Shows graphical visualizations of the TSP solution process and path optimization.

<h2>Running TSPGUI</h2>
To run the TSPGUI, first install the required dependencies with:

```
pip install -r requirements.txt
```

Next, the GUI needs a compile executable to run as a subprocess.
<h3>TSPSolver Executable</h3>
You can either download the executable binary in this repo's latest release, or compile the executable yourself.
Once you have the executable, place it in folder TSPGUI/SolverApplication/ (You will need to create this directory).

Finally run the script with:
```
python TSPGUI.py
```
