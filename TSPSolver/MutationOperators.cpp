#include "MutationOperators.h"
#include "Individual.h"
#include "RandomUtil.h"

#include <algorithm>
#include <random>

using namespace std;
using namespace RandomUtil;

void InsertMutation::mutate(Individual& individual) {
	// If dimesnion too low, mutation pointless
	if (individual.dimension <= 2) {
		return;
	}
	// Get two random points
	int samples[2];
	sampleTwo(individual.dimension, samples);
	int removeIndex = samples[0];
	int insertIndex = samples[1];
	City removedCity = individual.route[removeIndex]; // City to be moved

	// Remove moving city
	individual.route.erase(individual.route.begin() + removeIndex);
	// Relocate city to insert index
	individual.route.insert(individual.route.begin() + insertIndex, removedCity);
	// Recalculate fitness
	individual.calculateFitness();
}

void SwapMutation::mutate(Individual& individual) {
	// If dimesnion too low, mutation pointless
	if (individual.dimension <= 2) {
		return;
	}
	// Get two random points
	int samples[2];
	sampleTwo(individual.dimension, samples);
	// Directly swap cities
	swap(individual.route[samples[0]], individual.route[samples[1]]);
	// Recalculate fitness
	individual.calculateFitness();
}

void InversionMutation::mutate(Individual& individual) {
	// If dimesnion too low, mutation pointless
	if (individual.dimension <= 2) {
		return;
	}
	// Get two random points
	int samples[2];
	sampleTwo(individual.dimension, samples);
	// Sort samples
	if (samples[0] > samples[1]) {
		swap(samples[0], samples[1]);
	}
	// Reverse segment between samples
	reverse(individual.route.begin() + samples[0], individual.route.begin() + samples[1]);
	// Recalculate fitness
	individual.calculateFitness();
}

void ScrambleMutation::mutate(Individual& individual) {
	// If dimesnion too low, mutation pointless
	if (individual.dimension <= 2) {
		return;
	}
	// Get random point
	int startIndex = getRandomInt(0, individual.dimension - 1);
	// Get random length of segment, but make sure not out of bounds and within length
	// Too long a length is unlikely to produce meaningful results
	int segmentLength = getRandomInt(max(-startIndex, -maxSegmentLength), min(individual.dimension - 1 - startIndex, maxSegmentLength));
	// Shuffle vector differently based on if segment is to left or right of startIndex
	if (segmentLength > 0) {
		shuffleVector(individual.route, startIndex, startIndex + segmentLength);
	} else {
		shuffleVector(individual.route, startIndex + segmentLength, startIndex);
	}
	// Recalculate fitness
	individual.calculateFitness();
}
