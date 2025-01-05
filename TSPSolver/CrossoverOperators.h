#pragma once
#ifndef CROSSOVEROPERATORS_H
#define CROSSOVEROPERATORS_H

class Individual;

class CrossoverOperator {
	public:
		virtual Individual crossover(Individual& indA, Individual& indB) = 0;
};

class OrderCrossover : public CrossoverOperator {
	private:
		void reorder();
	public:
		Individual crossover(Individual& indA, Individual& indB) override;
};

class PMXCrossover : public CrossoverOperator {
	public:
		Individual crossover(Individual& indA, Individual& indB) override;
};

class CycleCrossover : public CrossoverOperator {
	public:
		Individual crossover(Individual& indA, Individual& indB) override;
};

class EdgeRecombinationCrossover : public CrossoverOperator {
	public:
		Individual crossover(Individual& indA, Individual& indB) override;
};

#endif // !CROSSOVEROPERATORS_H
