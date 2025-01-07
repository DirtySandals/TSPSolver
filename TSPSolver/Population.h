#pragma once

#ifndef POPULATION_H
#define POPULATION_H

#include "Individual.h"
#include "StatTracker.h"
#include "TSPProblem.h"

#include <string>
#include <vector>

class Population {
	public:
		std::vector<Individual> population;
		std::vector<City> problem;
		int populationSize;
		int dimension;

		Population();
		Population(std::vector<City> cities, int populationSize);
		~Population();
		void randomisePopulation();
		int calculateAllFitness();
		std::string fitnessesToString();
		size_t size();
};

#endif // !POPULATION_H