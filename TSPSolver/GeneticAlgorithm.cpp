#include "GeneticAlgorithm.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <typeinfo>
#include <vector>

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
	stats = StatTracker(dimension);
	population = new Population(tspProblem.problem, populationSize, &stats);
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

	cancelAlgorithm.store(false);
	runningAlg.store(false);
}

void GeneticAlgorithm::startGA(int maxGenerations) {
	if (dimension <= 3) {
		return;
	}
	cancelAlgorithm.store(false);
	runningAlg.store(true);

	unique_lock<mutex> lock(algMutex);

	thread algThread(&GeneticAlgorithm::runAlgorithm, this, maxGenerations);

	algStop.wait(lock, [this] { return this->cancelAlgorithm.load(); });

	algThread.join();
	runningAlg.store(false);
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
	cout << "Running Algorithm" << endl;
	int generation = 0;
	while (generation < maxGenerations && !cancelAlgorithm.load()) {
		if (abs(generation - stats.bestGeneration) >= generationTolerance) {
			stop();
			break;
		}

		runGeneration();

		generation++;
	}

	cout << "After " << generation << " generations, fittest individual: " << stats.bestFitness << endl;
}

void GeneticAlgorithm::runGeneration() {
	selector->select(*population, selectedPopulation);

	int end = populationSize;

	if (typeid(*selector) == typeid(ElitismSelection)) {
		int numElites = dynamic_cast<ElitismSelection*>(selector)->numElites;
		end = end - numElites;
		for (int i = 0; i <= numElites; i++) {
			population->addIndividual(selectedPopulation[i], i);
		}
	}

	stats.updateGen();

	random_device rd;
	default_random_engine eng(rd());
	uniform_real_distribution<float> distrFloat(0.0, 1.0);

	for (int i = 0; i < end; i += 2) {
		if (i == end - 1) {
			population->addIndividual(selectedPopulation[i], i);
			break;
		} else if (i > end - 1) {
			break;
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

			population->addIndividual(firstChild, i);
			population->addIndividual(secondChild, i + 1);
		} else {
			if (distrFloat(eng) < pm) {
				mutator->mutate(first);
			}
			if (distrFloat(eng) < pm) {
				mutator->mutate(second);
			}

			population->addIndividual(first, i);
			population->addIndividual(second, i + 1);
		}
	}
}