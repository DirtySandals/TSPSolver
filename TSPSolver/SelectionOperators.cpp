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

void SelectionOperator::copyPopulation(Population& population, Individual parents[]) {
	for (int i = 0; i < population.populationSize; i++) {
		parents[i] = population.population[i];
	}
}

void FitnessProportionalSelection::calculateWeightedFitness(Population& population) {
	float worst = population.stats->currentWorstFitness;

	for (int i = 0; i < population.populationSize; i++) {
		float fitness = abs(population.population[i].fitness - worst);

		fitnesses[i] = fitness;
	}
}

FitnessProportionalSelection::FitnessProportionalSelection(int populationSize) {
	fitnesses.resize(populationSize, 0);
}

void FitnessProportionalSelection::select(Population& population, Individual selectedIndividuals[]) {
	copyPopulation(population, selectedIndividuals);

	calculateWeightedFitness(population);

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
	copyPopulation(population, selectedIndividuals);
	if (population.populationSize < tournamentSize) {
		tournamentSize = population.populationSize;
	}

	for (int i = 0; i < population.populationSize; i++) {
		sample(population.populationSize, samples, tournamentSize);

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
	numElites = static_cast<int>((float) population.populationSize * eliteProportion);
	copyPopulation(population, selectedIndividuals);
	quickSort(selectedIndividuals, 0, population.populationSize);
}