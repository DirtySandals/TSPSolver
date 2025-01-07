#include "Population.h"
#include <iostream>

using namespace std;


Population::Population() {
}

Population::~Population() {
	population.clear();
	population.shrink_to_fit();
}

Population::Population(vector<City> cities, int populationSize) : populationSize(populationSize), problem(cities) {
	dimension = cities.size();

	for (int i = 0; i < populationSize; i++) {
		population.push_back(Individual(cities));
	}
	randomisePopulation();
	calculateAllFitness();
}

void Population::randomisePopulation() {
	for (int i = 0; i < populationSize; i++) {
		population[i].randomize();
	}
}

int Population::calculateAllFitness() {
	int bestFitnessIndex = 0;
	float bestFitness = population[0].fitness;

	for (int i = 1; i < populationSize; i++) {
		float fitness = population[i].calculateFitness();

		if (fitness < bestFitness) {
			bestFitness = fitness;
			bestFitnessIndex = i;
		}
	}

	return bestFitnessIndex;
}

size_t Population::size() {
	return population.size();
}

std::string Population::fitnessesToString() {
	string output = "Fitnesses: { ";

	for (int i = 0; i < populationSize; i++) {
		output += to_string(population[i].fitness);

		if (i != populationSize - 1) {
			output += ", ";
		}
	}

	output += " }";

	return output;
}
