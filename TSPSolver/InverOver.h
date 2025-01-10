#pragma once
#ifndef INVEROVER_H
#define INVEROVER_H

#include "GeneticAlgorithm.h"

// Derived class of GeneticAlgorithm, implements algorithm Inverover by Guo Tao and Zbigniew Michalewicz
class InverOver : public GeneticAlgorithm {
	private:
		// Member vectors to reduce memory allocation
		std::vector<City> currSol;
		std::vector<size_t> order;

		// Reverses section of solution
		void invert(size_t indexA, size_t indexB, std::vector<City>& solution);
		// Overrides runGeneration function
		void runGeneration() override;
	public:
		// Constructors
		InverOver();
		InverOver(TSPProblem& tspProblem, int populationSize);
};

#endif // !INVEROVER_H
