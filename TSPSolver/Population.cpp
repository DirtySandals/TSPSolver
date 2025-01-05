#include "Population.h"
#include <iostream>

using namespace std;


Population::Population() {
}

Population::~Population() {

}

Population::Population(TSPProblem tspProblem, int dimension) : dimension(dimension), problem(tspProblem.problem) {
	for (int i = 0; i < population_size; i++) {
		population.push_back(Individual(dimension, tspProblem.problem));
	}
	randomisePopulation();
}

void Population::randomisePopulation() {
	for (int i = 0; i < population_size; i++) {
		population[i].randomize();
	}
	calculateAllFitness();
}

void Population::calculateAllFitness() {
	for (int i = 0; i < population_size; i++) {
		if (population[i].fitness == -1) population[i].calculateFitness();
		if (i == 0) fittestFitness = population[i].fitness;
		fittestFitness = min(population[i].fitness, fittestFitness);
	}
	// stats
}

size_t Population::size() {
	return population.size();
}
