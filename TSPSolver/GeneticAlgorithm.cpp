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
// Delete all operators, population and dynamic array
GeneticAlgorithm::~GeneticAlgorithm() {
	delete population;
	delete mutator;
	delete crossover;
	delete selector;
	delete[] selectedPopulation;
}

GeneticAlgorithm::GeneticAlgorithm(TSPProblem& tspProblem, int populationSize, MutationOperator* mutator, CrossoverOperator* crossover, SelectionOperator* selector) {
	// Init variables
	this->populationSize = populationSize;
	dimension = tspProblem.problem.size();
	stats = StatTracker(dimension);
	population = new Population(tspProblem.problem, populationSize, &stats);

	selectedPopulation = new Individual[populationSize];

	this->mutator = mutator;
	this->crossover = crossover;
	this->selector = selector;
	// Calculate mutation and crossover probabilities
	initProbabilities();
	// Init atomic variables
	cancelAlgorithm.store(false);
	runningAlg.store(false);
}
// Start the genetic algorithm and run in separate thread. Used for asynchronous running of algorithm to stop early
void GeneticAlgorithm::startGA(int maxGenerations) {
	// If dimension too small, shortest path is solved automatically, no point
	if (dimension <= 3) {
		return;
	}
	// Assign atomic variables
	cancelAlgorithm.store(false);
	runningAlg.store(true);
	// Create lock
	unique_lock<mutex> lock(algMutex);
	// Run in separate thread
	thread algThread(&GeneticAlgorithm::runAlgorithm, this, maxGenerations);
	// Block flow until notified
	algStop.wait(lock, [this] { return this->cancelAlgorithm.load(); });
	// Join thread and set atomic variable to flag that algorithm finished
	algThread.join();
	runningAlg.store(false);
}
// Used to end algorithm early
void GeneticAlgorithm::stop() {
	cancelAlgorithm.store(true);
	// Notify blocking conditional variable
	algStop.notify_all();
}
// Calculate probabilities based on population size and dimension
void GeneticAlgorithm::initProbabilities() {
	float inversePopulationSize = 1.f / (float) populationSize;
	float inverseChromosomeLength = 1.f / (float) population->dimension;

	this->pc = (0.6f + 0.9f) / 2.f;
	this->pm = (inversePopulationSize + inverseChromosomeLength) / 2.f;
}
// Run genetic algorithm
void GeneticAlgorithm::runAlgorithm(int maxGenerations) {
	cout << "Running Algorithm" << endl;
	int generation = 0;
	while (generation < maxGenerations && !cancelAlgorithm.load()) {
		// If last best found solution found long ago, algorithm has converged on solution
		if (abs(generation - stats.bestGeneration) >= generationTolerance) {
			stop();
			break;
		}
		// Run algorithm
		runGeneration();

		generation++;
	}

	cout << "After " << generation << " generations, fittest individual: " << stats.bestFitness << endl;
}

void GeneticAlgorithm::runGeneration() {
	// Select parents
	selector->select(*population, selectedPopulation);

	int populationIndex = 0;
	int end = populationSize;
	// If selection is elitism, send elites through but allow for them to participate
	// in crossover via separation of int i and population index in next loop
	if (typeid(*selector) == typeid(ElitismSelection)) {
		int numElites = dynamic_cast<ElitismSelection*>(selector)->numElites;

		populationIndex = numElites + 1;
		end = end - numElites;
		// Guarantee elites survival
		for (int i = 0; i <= numElites; i++) {
			population->addIndividual(selectedPopulation[i], i);
		}
	}
	// Update stats for current gen
	stats.updateGen();
	// Setup random number generator
	random_device rd;
	default_random_engine eng(rd());
	uniform_real_distribution<float> distrFloat(0.0, 1.0);
	// Loop over each pair of individuals
	for (int i = 0; i < end; i += 2) {
		// If odd number of individuals left, let them through
		if (i == end - 1 || populationIndex == end - 1) {
			population->addIndividual(selectedPopulation[i], populationIndex);
			break;
		} else if (i > end - 1 || populationIndex > end - 1) {
			break;
		}

		Individual first = move(selectedPopulation[i]);
		Individual second = move(selectedPopulation[i + 1]);
		// If pair crossover
		if (distrFloat(eng) < pc) {
			// Crossover for children
			Individual firstChild = crossover->crossover(first, second);
			Individual secondChild = crossover->crossover(second, first);
			// Mutate children (if it happens)
			if (distrFloat(eng) < pm) {
				mutator->mutate(firstChild);
			}
			if (distrFloat(eng) < pm) {
				mutator->mutate(secondChild);
			}
			// Send children to next generation
			population->addIndividual(firstChild, populationIndex);
			population->addIndividual(secondChild, populationIndex + 1);
		} 
		// If pair do not crossover
		else {
			// Mutate individuals (if it happens)
			if (distrFloat(eng) < pm) {
				mutator->mutate(first);
			}
			if (distrFloat(eng) < pm) {
				mutator->mutate(second);
			}
			// Send individuals to next generation
			population->addIndividual(first, populationIndex);
			population->addIndividual(second, populationIndex + 1);
		}
		// Update populationIndex
		populationIndex += 2;
	}
}