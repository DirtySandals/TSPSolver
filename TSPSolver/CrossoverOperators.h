#pragma once
#ifndef CROSSOVEROPERATORS_H
#define CROSSOVEROPERATORS_H

#include <vector>
#include "Individual.h"

class Individual;
struct City;

// Crossover is a base class for crossover operators used in the genetic algorithm
class CrossoverOperator {
	protected:
		Individual child;
		void resetChild();
	public:
		CrossoverOperator(int dimension);
		// Pure virtual function for performing crossover across individuals
		virtual Individual crossover(Individual& indA, Individual& indB) = 0;
};

// Order Crossover takes a segment from one parent and inserts remaining entries in order
class OrderCrossover : public CrossoverOperator {
	private:
		std::vector<City> reorder(std::vector<City> route, std::vector<City> slice, int indexB);
	public:
		OrderCrossover(int dimension);
		Individual crossover(Individual& indA, Individual& indB) override;
};

// PMX Crossover takes a segment from one parent and partially maps the remaining entries into child
class PMXCrossover : public CrossoverOperator {
	public:
		PMXCrossover(int dimension);
		Individual crossover(Individual& indA, Individual& indB) override;
};

/*
 * Cycle Crossover cycles through the entries of each parent my mapping the City indexes to their indexes
 * in the parents. The child takes the route of odd cycles from parent A.
 */
class CycleCrossover : public CrossoverOperator {
	public:
		CycleCrossover(int dimension);
		Individual crossover(Individual& indA, Individual& indB) override;
};

#endif // !CROSSOVEROPERATORS_H
