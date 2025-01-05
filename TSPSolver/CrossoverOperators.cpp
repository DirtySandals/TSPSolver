#include "CrossoverOperators.h"

#include "Individual.h"
#include "RandomUtil.h"

#include <unordered_map>

using namespace std;
using namespace RandomUtil;

Individual OrderCrossover::crossover(Individual& indA, Individual& indB) {

}

Individual PMXCrossover::crossover(Individual& indA, Individual& indB) {
	unordered_map<int, int> indexMapB;
	int samples[2];
	sampleTwo(indA.dimension, samples);

	if (samples[0] > samples[1]) {
		swap(samples[0], samples[1]);
	}

	Individual

}

Individual CycleCrossover::crossover(Individual& indA, Individual& indB) {

}

Individual EdgeRecombinationCrossover::crossover(Individual& indA, Individual& indB) {

}
