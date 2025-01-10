#pragma once
#ifndef SELECTIONOPERATORS_H
#define SELECTIONOPERATORS_H

#include <vector>

class Population;
class Individual;

// Base class SelectionOperator is used for selection operators utilised in GeneticAlgorithm
class SelectionOperator {
	public:
		// Pure virtual function for selecting individuals in population
		virtual void select(Population& population, Individual selectedIndividuals[]) = 0;
};

// FitnessProportionalSelection selects from population using discrete weighted probability distribution
class FitnessProportionalSelection : public SelectionOperator {
	private:
		// Vector holding weighted fitnesses of population individuals
		std::vector<float> fitnesses;
		// Calculates the weighted fitnesses from population
		void calculateWeightedFitness(Population& population);
	public:
		FitnessProportionalSelection(int populationSize);
		void select(Population& population, Individual selectedIndividuals[]) override;
};

/*
 * TournamentSelection repeatedly randomly selects a number of individuals and the individual with
 * the best fitness survives. This is repeated until population is full
 */ 
class TournamentSelection : public SelectionOperator {
	private:
		int tournamentSize;
		// Holds dynamic array for sampling individual indexes
		int* samples = nullptr;
	public:
		TournamentSelection(int tournamentSize);
		~TournamentSelection();
		void select(Population& population, Individual selectedIndividuals[]) override;
};

/*
 * ElitismSelection sorts population in ascending order and top proportion of individuals
 * are guaranteed to survive into next generation
 */ 
class ElitismSelection : public SelectionOperator {
	private:
		float eliteProportion = 0.1f; // 10% of population is guaranteed to survive
		// Sorts individuals in ascending order according to their fitness O(nlogn)
		void quickSort(Individual selectedIndividuals[], int start, int end);
		// Copies population to parents array
		void copyPopulation(Population& population, Individual parents[]);
	public:
		int numElites; // Number of elites to survive into next generation

		void select(Population& population, Individual selectedIndividuals[]) override;
};

#endif // !SELECTIONOPERATORS_H
