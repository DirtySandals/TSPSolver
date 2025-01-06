#pragma once
#ifndef CROSSOVEROPERATORS_H
#define CROSSOVEROPERATORS_H

#include <vector>
#include "Individual.h"

class Individual;
struct City;

class CrossoverOperator {
	protected:
		Individual child;
		void resetChild();
	public:
		CrossoverOperator(int dimension);
		virtual Individual crossover(Individual& indA, Individual& indB) = 0;
};

class OrderCrossover : public CrossoverOperator {
	private:
		std::vector<City> reorder(std::vector<City> route, std::vector<City> slice, int indexB);
	public:
		OrderCrossover(int dimension);
		Individual crossover(Individual& indA, Individual& indB) override;
};

class PMXCrossover : public CrossoverOperator {
	public:
		PMXCrossover(int dimension);
		Individual crossover(Individual& indA, Individual& indB) override;
};

class CycleCrossover : public CrossoverOperator {
	public:
		CycleCrossover(int dimension);
		Individual crossover(Individual& indA, Individual& indB) override;
};

#endif // !CROSSOVEROPERATORS_H
