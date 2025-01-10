#pragma once

#ifndef POPULATION_H
#define POPULATION_H

#include "Individual.h"
#include "StatTracker.h"
#include "TSPProblem.h"

#include <string>
#include <limits>
#include <vector>

class Population {
	public:
		int bestFitnessIndex = -1;
		float bestFitness = std::numeric_limits<float>::max();

		std::vector<Individual> population;
		std::vector<City> problem;
		int populationSize;
		int dimension;

		Population();
		Population(std::vector<City> cities, int populationSize);
		~Population();
		void randomisePopulation();
		int calculateAllFitness();
		void addIndividual(Individual& ind, int index);
		std::string fitnessesToString();
		size_t size();
};

#endif // !POPULATION_H