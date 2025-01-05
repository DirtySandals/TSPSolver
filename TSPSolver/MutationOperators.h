#pragma once
#ifndef MUTATIONOPERATORS_H
#define MUTATIONOPERATORS_H

class Individual;

class MutationOperator {
	public:
		virtual void mutate(Individual& individual) = 0;
};

class InsertMutation : public MutationOperator {
	public:
		void mutate(Individual& individual) override;
};

class SwapMutation : public MutationOperator {
	public:
		void mutate(Individual& individual) override;
};

class InversionMutation : public MutationOperator {
public:
	void mutate(Individual& individual) override;
};


class ScrambleMutation : public MutationOperator {
	public:
		void mutate(Individual& individual) override;
};

#endif // !MUTATIONOPERATORS_H
