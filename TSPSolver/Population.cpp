#include "Population.h"

#include <iostream>
#include <stdexcept>

using namespace std;


Population::Population() {
}

Population::~Population() {
	// Remove memory
	population.clear();
	population.shrink_to_fit();
}

Population::Population(vector<City> cities, int populationSize, StatTracker* stats) : populationSize(populationSize) {
	// Init variables
	this->stats = stats;
	dimension = cities.size();
	// Init population
	for (int i = 0; i < populationSize; i++) {
		population.push_back(Individual(cities));
	}
	// Randomise population
	randomisePopulation();
	// Calculate all their fitnesses
	calculateAllFitness();
}

void Population::randomisePopulation() {
	for (int i = 0; i < populationSize; i++) {
		population[i].randomize();
	}
}

int Population::calculateAllFitness() {
	// Initially calculate all fitnesses and track best fitness
	int bestFitnessIndex = 0;
	float bestFitness = numeric_limits<float>::max();

	for (int i = 0; i < populationSize; i++) {
		float fitness = population[i].calculateFitness();

		if (fitness < bestFitness) {
			bestFitness = fitness;
			bestFitnessIndex = i;
		}
	}
	// Initialise stats
	stats->update(population[bestFitnessIndex].route, bestFitness);

	return bestFitnessIndex;
}

std::string Population::fitnessesToString() {
	// Create string of fitnesses of population for printing purposes
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
	// Add individual to population
	population[index] = move(ind);
	// Update stats to track fitnesses
	stats->update(ind.route, ind.fitness);
}
