#include "CrossoverOperators.h"

#include "Individual.h"
#include "RandomUtil.h"

#include <set>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace RandomUtil;

void CrossoverOperator::resetChild() {
	for (int i = 0; i < child.route.size(); i++) {
		child.route[i].index = -1;
	}
}

CrossoverOperator::CrossoverOperator(int dimension) {
	vector<City> cities;
	for (int i = 0; i < dimension; i++) {
		cities.push_back(City(1, 1, 1));
	}

	child = Individual(dimension, cities);
}


std::vector<City> OrderCrossover::reorder(std::vector<City> route, std::vector<City> slice, int indexB) {
	vector<City> orderedCities;
	orderedCities.reserve(route.size());

	orderedCities.insert(orderedCities.end(), route.begin() + indexB + 1, route.end());

	orderedCities.insert(orderedCities.end(), route.begin(), route.begin() + indexB + 1);

	set<int> sliceSet;
	
	for (City& city : slice) {
		sliceSet.insert(city.index);
	}

	vector<City> output;
	output.reserve(route.size());

	for (City& city : orderedCities) {
		if (sliceSet.find(city.index) != sliceSet.end()) {
			continue;
		}

		output.push_back(city);
	}

	return output;
}

OrderCrossover::OrderCrossover(int dimension) : CrossoverOperator(dimension) {

}

Individual OrderCrossover::crossover(Individual& indA, Individual& indB) {
	resetChild();

	int samples[2];
	sampleTwo(indA.dimension, samples);

	if (samples[0] > samples[1]) {
		swap(samples[0], samples[1]);
	}

	for (int i = samples[0]; i <= samples[1]; i++) {
		child.route[i] = indA.route[i];
	}

	vector<City> sliceA;
	sliceA.reserve(indA.dimension);

	sliceA.insert(sliceA.end(), indA.route.begin() + samples[0], indA.route.begin() + samples[1] + 1);

	vector<City> orderedCities = reorder(indB.route, sliceA, samples[1]);


	for (int i = 0; i < orderedCities.size(); i++) {
		int idx = (samples[1] + 1 + i) % indA.dimension;
		child.route[idx] = orderedCities[i];
	}

	return child;
}

PMXCrossover::PMXCrossover(int dimension) : CrossoverOperator(dimension) {
}

Individual PMXCrossover::crossover(Individual& indA, Individual& indB) {
	resetChild();

	unordered_map<int, int> indexMapB;
	set<int> sublistSet;
	int samples[2];
	sampleTwo(indA.dimension, samples);

	if (samples[0] > samples[1]) {
		swap(samples[0], samples[1]);
	}

	for (int i = samples[0]; i <= samples[1]; i++) {
		child.route[i] = indA.route[i];
		sublistSet.insert(indA.route[i].index);
	}

	for (int i = 0; i < indB.dimension; i++) {
		indexMapB[indB.route[i].index] = i;
	}

	for (int i = samples[0]; i <= samples[1]; i++) {
		if (sublistSet.find(indB.route[i].index) != sublistSet.end()) {
			continue;
		}

		int mappedBIndex = i;

		while (samples[0] <= mappedBIndex && mappedBIndex <= samples[1]) {
			City cityA = indA.route[mappedBIndex];
			mappedBIndex = indexMapB[cityA.index];
		}

		child.route[mappedBIndex] = indB.route[i];
	}

	for (int i = 0; i < indA.dimension; i++) {
		if (child.route[i].index == -1) {
			child.route[i] = indB.route[i];
		}
	}

	return child;
}

CycleCrossover::CycleCrossover(int dimension) : CrossoverOperator(dimension) {
}

Individual CycleCrossover::crossover(Individual& indA, Individual& indB) {
	resetChild();

	unordered_map<int, int> indexAMap;

	for (int i = 0; i < indA.dimension; i++) {
		indexAMap[indA.route[i].index] = i;
	}

	int cycle = 0;
	int lastCycleStart = 0;
	int idx = 0;
	set<int> indexesVisited;

	while (indexesVisited.size() != indA.dimension) {
		for (int i = lastCycleStart; i < indA.dimension; i++) {
			if (indexesVisited.find(i) != indexesVisited.end()) {
				continue;
			}
			idx = i;

			for (int j = i + 1; i < indA.dimension; j++) {
				if (indexesVisited.find(j) == indexesVisited.end()) {
					lastCycleStart = j;
					break;
				}
			}
			break;
		}

		while (indexesVisited.find(idx) == indexesVisited.end()) {
			child.route[idx] = cycle % 2 == 0 ? indA.route[idx] : indB.route[idx];

			indexesVisited.insert(idx);

			idx = indexAMap[indB.route[idx].index];
		}

		cycle += 1;
	}

	return child;
}