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

#include <limits>

class GeneticAlgorithm {
	protected:
		float pc = 0.f;
		float pm = 0.f;
		Population* population = nullptr;
		int population_size;
		int dimension;
		std::vector<City> problem;
		std::vector<City> currSol;
		std::vector<size_t> order;

		MutationOperator* mutator = nullptr;
		CrossoverOperator* crossover = nullptr;
		SelectionOperator* selector = nullptr;

		Individual* selectedPopulation = nullptr;

		float fittest = std::numeric_limits<float>::max();

		void initProbabilities();
		virtual void runGeneration();
	public:
		GeneticAlgorithm();
		~GeneticAlgorithm();
		GeneticAlgorithm(TSPProblem& tspProblem, MutationOperator* mutator, CrossoverOperator* crossover, SelectionOperator* selector);
		float startGA(int maxGenerations=10000);
};

#endif // !GENETICALGORITHM_H

