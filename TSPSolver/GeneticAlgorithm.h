#pragma once

#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "City.h"
#include "CrossoverOperators.h"
#include "Individual.h"
#include "MutationOperators.h"
#include "Population.h"
#include "SelectionOperators.h"
#include "TSPProblem.h"

#include <atomic>
#include <condition_variable>
#include <limits>
#include <mutex>

/*
 * GeneticAlgorithm performs a genetic algorithm on a given TSP instance.
 * It is provided the necessary mutation, crossover and selection operators
 * required to perform the genetic algorithm
 */
class GeneticAlgorithm {
	protected:
		// Controls how long to keep running algorithm after no improvment in best solution
		static const int generationTolerance = 1000;

		StatTracker stats; // Used to track the stats of current generation and entire lifecycle

		// Probabilities (calculated in constructor)
		float pc = 0.f; // Crossover probability
		float pm = 0.f; // Mutation probability

		// Population contains population of solutions
		Population* population = nullptr;
		// Problem variables
		int populationSize;
		int dimension;

		// Algorithm operators
		MutationOperator* mutator = nullptr;
		CrossoverOperator* crossover = nullptr;
		SelectionOperator* selector = nullptr;

		// Dynamic array of individuals made for less dynamic memory allocation
		Individual* selectedPopulation = nullptr;

		// Variables for determining whether algorithm has finished to control thread management
		std::atomic<bool> cancelAlgorithm;
		std::mutex algMutex;
		std::condition_variable algStop;

		// Initialises the probabilities of mutation and crossover
		void initProbabilities();
		// Performs the genetic algorithm
		void runAlgorithm(int maxGenerations);
		// Computes a generation of the genetic algorithm
		virtual void runGeneration();
	public:
		std::atomic<bool> runningAlg; // Atomic variable to determine if algorithm is still running
		GeneticAlgorithm(); // Default constructor
		~GeneticAlgorithm(); // Default destructor
		// Constructor for outlining problem and desired operators
		GeneticAlgorithm(TSPProblem& tspProblem, int populationSize, MutationOperator* mutator, CrossoverOperator* crossover, SelectionOperator* selector);
		// Used for starting genetic algorithm, begins thread for runAlgorithm
		void startGA(int maxGenerations=std::numeric_limits<int>::max());
		// Stops genetic algorithm thread
		void stop();
};

#endif // !GENETICALGORITHM_H

