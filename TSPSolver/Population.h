#pragma once

#ifndef POPULATION_H
#define POPULATION_H

#include "Individual.h"
#include "StatTracker.h"
#include "TSPProblem.h"

#include <string>
#include <limits>
#include <vector>

// Population abstracts all the solutions contained in the genetic algorithm
class Population {
	public:
		StatTracker* stats = nullptr; // Pointer to stats

		std::vector<Individual> population; // Vector containing all solutions

		int populationSize; // Number of solutions
		int dimension; // Number of cities in problem

		Population();
		Population(std::vector<City> cities, int populationSize, StatTracker* stats);
		~Population();
		// Randomises all solutions' routes
		void randomisePopulation();
		// Initialises the fitness of all solutions
		int calculateAllFitness();
		// Used for inserting individual in population
		void addIndividual(Individual& ind, int index);
		// Returns string listing all fitnesses of population
		std::string fitnessesToString();
};

#endif // !POPULATION_H