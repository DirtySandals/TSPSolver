#pragma once
#ifndef INVEROVER_H
#define INVEROVER_H

#include "GeneticAlgorithm.h"

class InverOver : public GeneticAlgorithm {
	private:
		void invert(size_t indexA, size_t indexB, std::vector<City>& solution);
		void runGeneration();
	public:
		InverOver();
		InverOver(TSPProblem& tspProblem, int populationSize);
};

#endif // !INVEROVER_H
