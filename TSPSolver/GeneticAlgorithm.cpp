#include "GeneticAlgorithm.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>
#include <fstream>

using namespace std;

GeneticAlgorithm::GeneticAlgorithm() {
}

GeneticAlgorithm::~GeneticAlgorithm() {
	delete population;
	delete mutator;
	delete crossover;
	delete selector;
	delete[] selectedPopulation;
}

GeneticAlgorithm::GeneticAlgorithm(TSPProblem& tspProblem, int populationSize, MutationOperator* mutator, CrossoverOperator* crossover, SelectionOperator* selector) : problem(tspProblem.problem) {
	populationSize = 50;
	dimension = tspProblem.dimension;
	population = new Population(tspProblem.problem, populationSize, dimension);
	currSol.reserve(dimension);
	order.reserve(dimension);

	selectedPopulation = new Individual[populationSize];

	this->mutator = mutator;
	this->crossover = crossover;
	this->selector = selector;

	for (int i = 0; i < tspProblem.dimension; i++) {
		currSol.push_back(City());
		order.push_back(i);
	}

	initProbabilities();
}

float GeneticAlgorithm::startGA(int maxGenerations) {
	int generation = 0;
	while (generation < maxGenerations) {
		runGeneration();
		fittest = min(population->fittestFitness, fittest);
		if (generation % 1000 == 0) {
			cout << "Generation: " << generation + 1 << endl;
			cout << "fittest: " << fittest << endl;
		}
		generation++;
	}
	cout << "fittest: " << fittest << endl;
	return fittest;
}

void GeneticAlgorithm::initProbabilities() {
	float inversePopulationSize = 1.f / (float) populationSize;
	float inverseChromosomeLength = 1.f / (float)population->dimension;

	this->pc = (0.6f + 0.9f) / 2.f;
	this->pm = (inversePopulationSize + inverseChromosomeLength) / 2.f;
}

void GeneticAlgorithm::runGeneration() {
	population->calculateAllFitness();

	selector->select(*population, selectedPopulation);

	random_device rd;
	default_random_engine eng(rd());
	uniform_real_distribution<float> distrFloat(0.0, 1.0);

	for (int i = 0; i < populationSize; i += 2) {
		if (i + 1 == populationSize) {
			population->population[i] = selectedPopulation[i];
		}
		Individual first = selectedPopulation[i];
		Individual second = selectedPopulation[i + 1];

		if (distrFloat(eng) < pc) {
			Individual firstChild = crossover->crossover(first, second);
			Individual secondChild = crossover->crossover(second, first);

			if (distrFloat(eng) < pm) {
				mutator->mutate(firstChild);
			}
			if (distrFloat(eng) < pm) {
				mutator->mutate(secondChild);
			}
			firstChild.calculateFitness();
			secondChild.calculateFitness();
			population->population[i] = firstChild;
			population->population[i + 1] = secondChild;

		} else {
			if (distrFloat(eng) < pm) {
				mutator->mutate(first);
			}
			if (distrFloat(eng) < pm) {
				mutator->mutate(second);
			}

			population->population[i] = first;
			population->population[i + 1] = second;
		}
	}
}