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
	const int population_size = 50;
	int dimension;
	float fittestFitness;
	Population();
	Population(TSPProblem tspProblem, int dimension);
	~Population();
	void randomisePopulation();
	void calculateAllFitness();
	size_t size();
};

#endif // !POPULATION_H