#include "MutationOperators.h"
#include "Individual.h"
#include "RandomUtil.h"

#include <algorithm>
#include <random>

using namespace std;
using namespace RandomUtil;

void InsertMutation::mutate(Individual& individual) {
	if (individual.dimension <= 2) {
		return;
	}

	int samples[2];
	sampleTwo(individual.dimension, samples);
	int removeIndex = samples[0];
	int insertIndex = samples[1];
	City removedCity = individual.route[removeIndex];

	individual.route.erase(individual.route.begin() + removeIndex);

	individual.route.insert(individual.route.begin() + insertIndex, removedCity);
}

void SwapMutation::mutate(Individual& individual) {
	if (individual.dimension <= 2) {
		return;
	}

	int samples[2];
	sampleTwo(individual.dimension, samples);

	swap(individual.route[samples[0]], individual.route[samples[1]]);
}

void InversionMutation::mutate(Individual& individual) {
	if (individual.dimension <= 2) {
		return;
	}

	int samples[2];
	sampleTwo(individual.dimension, samples);

	if (samples[0] > samples[1]) {
		swap(samples[0], samples[1]);
	}

	reverse(individual.route.begin() + samples[0], individual.route.begin() + samples[1]);
}

void ScrambleMutation::mutate(Individual& individual) {
	if (individual.dimension <= 2) {
		return;
	}

	int samples[2];
	sampleTwo(individual.dimension, samples);

	if (samples[0] > samples[1]) {
		swap(samples[0], samples[1]);
	}

	shuffleVector(individual.route, samples[0], samples[1]);
}
