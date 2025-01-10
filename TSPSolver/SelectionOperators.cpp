#include "SelectionOperators.h"

#include "Individual.h"
#include "Population.h"
#include "RandomUtil.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace RandomUtil;
using namespace std;

void FitnessProportionalSelection::calculateWeightedFitness(Population& population) {
	// Weight every thing relative to distance of worst fitness (best fitness becomes highest)
	float worst = population.stats->currentWorstFitness;

	for (int i = 0; i < population.populationSize; i++) {
		// Inclusion of 0.1f to avoid probabilities of all zeroes (violating discrete random distribution |p| = 1)
		// Also gives slim chance for worst solutions
		float fitness = max(abs(population.population[i].fitness - worst), 0.1f);
		fitnesses[i] = fitness;
	}
}

FitnessProportionalSelection::FitnessProportionalSelection(int populationSize) {
	// Intialise vector to avoid memory allocation later on
	fitnesses.resize(populationSize, 0);
}

void FitnessProportionalSelection::select(Population& population, Individual selectedIndividuals[]) {
	// Create weighted probabilties
	calculateWeightedFitness(population);
	// Populate selectedIndividuals with population individuals via weighted discrete random distribution
	weightDistribution(population.population, fitnesses, selectedIndividuals);
}

TournamentSelection::TournamentSelection(int tournamentSize) {
	// Initialise array for sampling indexes
	this->tournamentSize = tournamentSize;
	samples = new int[tournamentSize];
}

TournamentSelection::~TournamentSelection() {
	// Deallocate samples array
	delete[] samples;
}

void TournamentSelection::select(Population& population, Individual selectedIndividuals[]) {
	// If population size somehow lower than tournament size, resize tournament size
	if (population.populationSize < tournamentSize) {
		tournamentSize = population.populationSize;
	}

	// For each selectedIndividual, make a tournament for tournamentSize number of individuals with minimum
	// fitness surviving to selectedIndividuals
	for (int i = 0; i < population.populationSize; i++) {
		// Get tournamentSize samples
		sample(population.populationSize, samples, tournamentSize);
		// Find best/min fitness
		int minIndex = samples[0];
		int minFitness = population.population[samples[0]].fitness;
		for (int i = 1; i < tournamentSize; i++) {
			if (population.population[samples[i]].fitness < minFitness) {
				minIndex = samples[i];
				minFitness = population.population[samples[i]].fitness;
			}
		}
		// Best individual survives
		selectedIndividuals[i] = move(population.population[minIndex]);
	}
}

// Standard quicksort
void ElitismSelection::quickSort(Individual selectedIndividuals[], int start, int end) {
	if (start >= end)
		return;

	int pivotSpot = end - 1;
	Individual pivot = selectedIndividuals[pivotSpot];
	int pivotIndex = start;

	for (int i = start; i < end; i++) {
		if (selectedIndividuals[i].fitness < pivot.fitness) {
			swap(selectedIndividuals[i], selectedIndividuals[pivotIndex]);

			pivotIndex++;
		}
	}

	swap(selectedIndividuals[pivotSpot], selectedIndividuals[pivotIndex]);

	quickSort(selectedIndividuals, start, pivotIndex);
	quickSort(selectedIndividuals, pivotIndex + 1, end);
}

void ElitismSelection::copyPopulation(Population& population, Individual parents[]) {
	// Intialise parents/selectedIndividuals to population
	for (int i = 0; i < population.populationSize; i++) {
		parents[i] = population.population[i];
	}
}

void ElitismSelection::select(Population& population, Individual selectedIndividuals[]) {
	// Calculate number of elites to progress to next generation
	numElites = static_cast<int>((float) population.populationSize * eliteProportion);
	// Copy population to selectedIndividuals
	copyPopulation(population, selectedIndividuals);
	// Sort individuals in ascending order based on fitness
	quickSort(selectedIndividuals, 0, population.populationSize);
}