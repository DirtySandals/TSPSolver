#pragma once
#ifndef MUTATIONOPERATORS_H
#define MUTATIONOPERATORS_H

class Individual;

// MutationOperator is a base class for mutation operators in the genetic algorithm
class MutationOperator {
	public:
		// Pure virtual function for mutating individual
		virtual void mutate(Individual& individual) = 0;
};

// InsertMutation selects two random entries and inserts one next to the other
class InsertMutation : public MutationOperator {
	public:
		void mutate(Individual& individual) override;
};

// SwapMutation swaps two random entries
class SwapMutation : public MutationOperator {
	public:
		void mutate(Individual& individual) override;
};

// InversionMutation takes a random segment from route and reverses its order
class InversionMutation : public MutationOperator {
public:
	void mutate(Individual& individual) override;
};

// ScrambleMutation takes a random segment from route and shuffles its order
class ScrambleMutation : public MutationOperator {
	private:
		int maxSegmentLength = 8; // Maximum length of segment during mutation
	public:
		void mutate(Individual& individual) override;
};

#endif // !MUTATIONOPERATORS_H
