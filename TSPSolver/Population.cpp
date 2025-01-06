#include "Population.h"
#include <iostream>

using namespace std;


Population::Population() {
}

Population::~Population() {

}

Population::Population(vector<City> cities, int populationSize, int dimension) : populationSize(populationSize), dimension(dimension), problem(cities) {
	for (int i = 0; i < populationSize; i++) {
		population.push_back(Individual(dimension, cities));
	}
	randomisePopulation();
	calculateAllFitness();
}

void Population::randomisePopulation() {
	for (int i = 0; i < populationSize; i++) {
		population[i].randomize();
	}
	calculateAllFitness();
}

void Population::calculateAllFitness() {
	for (int i = 0; i < populationSize; i++) {
		if (population[i].fitness == -1) population[i].calculateFitness();
		if (i == 0) fittestFitness = population[i].fitness;
		fittestFitness = min(population[i].fitness, fittestFitness);
	}
}

size_t Population::size() {
	return population.size();
}
