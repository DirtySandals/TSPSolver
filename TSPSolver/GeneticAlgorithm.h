#pragma once

#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "City.h"
#include "CrossoverOperators.h"
#include "Individual.h"
#include "MutationOperators.h"
#include "Population.h"
#include "SelectionOperators.h"
#include "TSPProblem.h"

#include <atomic>
#include <condition_variable>
#include <limits>
#include <mutex>

class GeneticAlgorithm {
	protected:
		StatTracker stats;

		float pc = 0.f;
		float pm = 0.f;

		Population* population = nullptr;
		int populationSize;
		int dimension;

		std::vector<City> problem;
		std::vector<City> currSol;
		std::vector<size_t> order;

		MutationOperator* mutator = nullptr;
		CrossoverOperator* crossover = nullptr;
		SelectionOperator* selector = nullptr;

		Individual* selectedPopulation = nullptr;

		std::atomic<bool> cancelAlgorithm;
		std::mutex algMutex;
		std::condition_variable algStop;
		static const int generationTolerance = 10000;

		void initProbabilities();
		void runAlgorithm(int maxGenerations);
		virtual void runGeneration();
	public:
		std::atomic<bool> runningAlg;
		GeneticAlgorithm();
		~GeneticAlgorithm();
		GeneticAlgorithm(TSPProblem& tspProblem, int populationSize, MutationOperator* mutator, CrossoverOperator* crossover, SelectionOperator* selector);
		void startGA(int maxGenerations=std::numeric_limits<int>::max());
		void stop();
};

#endif // !GENETICALGORITHM_H

