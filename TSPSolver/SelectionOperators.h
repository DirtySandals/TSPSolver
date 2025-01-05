#pragma once
#ifndef SELECTIONOPERATORS_H
#define SELECTIONOPERATORS_H

class Population;
class Individual;

class SelectionOperator {
	public:
		virtual void select(Population& population, Individual selectedIndividuals[]) = 0;
};

class FitnessProportionalSelection : public SelectionOperator {
	public:
		void select(Population& population, Individual selectedIndividuals[]) override;
};

class TournamentSelection : public SelectionOperator {
	public:
		void select(Population& population, Individual selectedIndividuals[]) override;
};

class ElitismSelection : public SelectionOperator {
	public:
		void select(Population& population, Individual selectedIndividuals[]) override;
};

#endif // !SELECTIONOPERATORS_H
