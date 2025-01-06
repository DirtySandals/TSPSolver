#pragma once

#ifndef POPULATION_H
#define POPULATION_H

#include "Individual.h"
#include "TSPProblem.h"

#include <vector>

class Population {
public:
	std::vector<Individual> population;
	std::vector<City> problem;
	int populationSize;
	int dimension;
	float fittestFitness;
	Population();
	Population(std::vector<City> cities, int populationSize, int dimension);
	~Population();
	void randomisePopulation();
	void calculateAllFitness();
	size_t size();
};

#endif // !POPULATION_H