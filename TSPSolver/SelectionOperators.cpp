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

FitnessProportionalSelection::FitnessProportionalSelection(int populationSize) {
	for (int i = 0; i < populationSize; i++) {
		fitnesses.push_back(0);
	}
}

void FitnessProportionalSelection::select(Population& population, Individual selectedIndividuals[]) {
	for (int i = 0; i < population.populationSize; i++) {
		fitnesses[i] = 1.f / population.population[i].fitness;
	}

	weightDistribution(population.population, fitnesses, selectedIndividuals);
}

TournamentSelection::TournamentSelection(int tournamentSize) {
	this->tournamentSize = tournamentSize;
	samples = new int[tournamentSize];
}

TournamentSelection::~TournamentSelection() {
	delete[] samples;
}

void TournamentSelection::select(Population& population, Individual selectedIndividuals[]) {
	if (population.dimension < tournamentSize) {
		tournamentSize = population.dimension;
	}

	for (int i = 0; i < population.populationSize; i++) {
		sample(population.dimension, samples, tournamentSize);

		int minIndex = samples[0];
		int minFitness = population.population[samples[0]].fitness;
		for (int i = 1; i < tournamentSize; i++) {
			if (population.population[samples[i]].fitness < minFitness) {
				minIndex = samples[i];
				minFitness = population.population[samples[i]].fitness;
			}
		}
		selectedIndividuals[i] = move(population.population[minIndex]);
	}
}

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

void ElitismSelection::select(Population& population, Individual selectedIndividuals[]) {
	quickSort(selectedIndividuals, 0, population.dimension);
}
