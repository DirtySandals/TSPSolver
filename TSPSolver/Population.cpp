#include "Population.h"

#include <iostream>
#include <stdexcept>

using namespace std;


Population::Population() {
}

Population::~Population() {
	population.clear();
	population.shrink_to_fit();
}

Population::Population(vector<City> cities, int populationSize, StatTracker* stats) : populationSize(populationSize), problem(cities) {
	this->stats = stats;
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
	float bestFitness = numeric_limits<float>::max();
	float max = numeric_limits<float>::min();

	for (int i = 0; i < populationSize; i++) {
		float fitness = population[i].calculateFitness();

		if (fitness < bestFitness) {
			bestFitness = fitness;
			bestFitnessIndex = i;
		}
		if (fitness > max) {
			max = fitness;
		}
	}

	stats->update(population[bestFitnessIndex].route, bestFitness);

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

void Population::addIndividual(Individual& ind, int index) {
	if (index < 0 || index >= populationSize) {
		throw out_of_range("Index: " + to_string(index) + " is out of population range");
	}

	population[index] = move(ind);

	stats->update(ind.route, ind.fitness);
}
