#pragma once
#ifndef SELECTIONOPERATORS_H
#define SELECTIONOPERATORS_H

#include <vector>

class Population;
class Individual;

class SelectionOperator {
	protected:
		void copyPopulation(Population& population, Individual parents[]);
	public:
		virtual void select(Population& population, Individual selectedIndividuals[]) = 0;
};

class FitnessProportionalSelection : public SelectionOperator {
	private:
		std::vector<float> fitnesses;

		void calculateWeightedFitness(Population& population);
	public:
		FitnessProportionalSelection(int populationSize);
		void select(Population& population, Individual selectedIndividuals[]) override;
};

class TournamentSelection : public SelectionOperator {
	private:
		int tournamentSize;
		int* samples = nullptr;
	public:
		TournamentSelection(int tournamentSize);
		~TournamentSelection();
		void select(Population& population, Individual selectedIndividuals[]) override;
};

class ElitismSelection : public SelectionOperator {
	private:
		float eliteProportion = 0.2f;
		void quickSort(Individual selectedIndividuals[], int start, int end);
	public:
		int numElites;

		void select(Population& population, Individual selectedIndividuals[]) override;
};

#endif // !SELECTIONOPERATORS_H
