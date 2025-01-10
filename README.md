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
