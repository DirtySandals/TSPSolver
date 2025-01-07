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
	this->populationSize = populationSize;
	dimension = tspProblem.problem.size();
	population = new Population(tspProblem.problem, populationSize);
	currSol.reserve(dimension);
	order.reserve(dimension);

	stats = StatTracker(dimension);

	selectedPopulation = new Individual[populationSize];

	this->mutator = mutator;
	this->crossover = crossover;
	this->selector = selector;
	
	for (int i = 0; i < tspProblem.dimension; i++) {
		currSol.push_back(City());
		order.push_back(i);
	}

	initProbabilities();

	cancelAlgorithm.store(false);
}

void GeneticAlgorithm::startGA(int maxGenerations) {
	cancelAlgorithm.store(false);

	unique_lock<mutex> lock(algMutex);

	thread algThread(&GeneticAlgorithm::runAlgorithm, this, maxGenerations);

	algStop.wait(lock, [this] { return this->cancelAlgorithm.load(); });

	algThread.join();
}

void GeneticAlgorithm::stop() {
	cancelAlgorithm.store(true);
	algStop.notify_all();
}

void GeneticAlgorithm::initProbabilities() {
	float inversePopulationSize = 1.f / (float) populationSize;
	float inverseChromosomeLength = 1.f / (float) population->dimension;

	this->pc = (0.6f + 0.9f) / 2.f;
	this->pm = (inversePopulationSize + inverseChromosomeLength) / 2.f;
}

void GeneticAlgorithm::runAlgorithm(int maxGenerations) {
	int generation = 0;
	while (generation < maxGenerations && !cancelAlgorithm.load()) {
		if (abs(generation - stats.bestGeneration) >= generationTolerance) {
			stop();
			break;
		}

		runGeneration();

		stats.update(population->population[bestFitnessIndex].route, population->population[bestFitnessIndex].fitness, generation);

		generation++;
	}

	cout << "After " << generation << " generations, fittest individual: " << stats.bestFitness << endl;
}

void GeneticAlgorithm::runGeneration() {
	bestFitnessIndex = population->calculateAllFitness();

	selector->select(*population, selectedPopulation);

	random_device rd;
	default_random_engine eng(rd());
	uniform_real_distribution<float> distrFloat(0.0, 1.0);

	for (int i = 0; i < populationSize; i += 2) {
		if (i >= populationSize - 1) {
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

			population->population[i] = move(firstChild);
			population->population[i + 1] = move(secondChild);
		} else {
			if (distrFloat(eng) < pm) {
				mutator->mutate(first);
				first.calculateFitness();
				population->population[i] = move(first);
			}
			if (distrFloat(eng) < pm) {
				mutator->mutate(second);
				second.calculateFitness();
				population->population[i + 1] = move(second);
			}
		}
	}
}