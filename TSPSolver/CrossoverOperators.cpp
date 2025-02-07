#include "CrossoverOperators.h"

#include "Individual.h"
#include "RandomUtil.h"

#include <set>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace RandomUtil;

// Initialises each index from child to -1 to show city has now been accessed/assigned
void CrossoverOperator::resetChild() {
	for (City& city : child.route) {
		city.index = -1;
	}
}

CrossoverOperator::CrossoverOperator(int dimension) {
	vector<City> cities;
	// Initialise child route with empty cities
	for (int i = 0; i < dimension; i++) {
		cities.push_back(City());
	}

	child = Individual(cities);
}


std::vector<City> OrderCrossover::reorder(std::vector<City> route, std::vector<City> slice, int indexB) {
	// Insert all cities from route ordered from end of segment, wrapping around to before the end of segment
	vector<City> orderedCities;
	orderedCities.reserve(route.size());
	orderedCities.insert(orderedCities.end(), route.begin() + indexB + 1, route.end());
	orderedCities.insert(orderedCities.end(), route.begin(), route.begin() + indexB + 1);

	// Create set of city indexes within segment/slice
	set<int> sliceSet;
	
	for (City& city : slice) {
		sliceSet.insert(city.index);
	}
	// Iterating in orderedCities order, create vector listing all cities not found in slice
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
// Calls base constructor
OrderCrossover::OrderCrossover(int dimension) : CrossoverOperator(dimension) {

}

Individual OrderCrossover::crossover(Individual& indA, Individual& indB) {
	// Reset child's indexes to -1
	resetChild();
	// If dimension too low, crossover useless
	if (indA.dimension <= 2) {
		child.route = indA.route;
		child.fitness = indA.fitness;
		return child;
	}
	// Samples two points in route for segment
	int samples[2];
	sampleTwo(indA.dimension, samples);
	// Sort samples
	if (samples[0] > samples[1]) {
		swap(samples[0], samples[1]);
	}
	// Transfer segment from indA to child
	for (int i = samples[0]; i <= samples[1]; i++) {
		child.route[i] = indA.route[i];
	}
	// Make vector of segment
	vector<City> sliceA;
	sliceA.reserve(indA.dimension);

	sliceA.insert(sliceA.end(), indA.route.begin() + samples[0], indA.route.begin() + samples[1] + 1);
	// Get remaining cities in order from after segment
	vector<City> orderedCities = reorder(indB.route, sliceA, samples[1]);
	
	// Starting in order after segment, insert cities from indB
	for (int i = 0; i < orderedCities.size(); i++) {
		int idx = (samples[1] + 1 + i) % indA.dimension;
		child.route[idx] = orderedCities[i];
	}
	// Recalculate fitness
	child.calculateFitness();

	return child;
}

PMXCrossover::PMXCrossover(int dimension) : CrossoverOperator(dimension) {
}

Individual PMXCrossover::crossover(Individual& indA, Individual& indB) {
	// Reset child's indexes to -1
	resetChild();
	// If dimension too low, crossover useless
	if (indA.dimension <= 2) {
		child.route = indA.route;
		child.fitness = indA.fitness;
		return child;
	}

	unordered_map<int, int> indexMapB;
	set<int> sublistSet;
	// Samples two points in route for segment
	int samples[2];
	sampleTwo(indA.dimension, samples);
	// Sort samples
	if (samples[0] > samples[1]) {
		swap(samples[0], samples[1]);
	}
	// Copy cities from indA in segment formed by sample indexes
	for (int i = samples[0]; i <= samples[1]; i++) {
		child.route[i] = indA.route[i];
		// Add city to set of sublist
		sublistSet.insert(indA.route[i].index);
	}
	// Map city indexes from indB to their route indexes
	for (int i = 0; i < indB.dimension; i++) {
		indexMapB[indB.route[i].index] = i;
	}

	// Iterate through segment, mapping indB cities in segment indexes not found in segment to indexes outside segment
	for (int i = samples[0]; i <= samples[1]; i++) {
		// If city in segment, no need to relocate city via mapping
		if (sublistSet.find(indB.route[i].index) != sublistSet.end()) {
			continue;
		}

		int mappedBIndex = i;
		// While mappedBIndex in the segment
		while (samples[0] <= mappedBIndex && mappedBIndex <= samples[1]) {
			// Map city in A segment to its index in B
			City cityA = indA.route[mappedBIndex];
			mappedBIndex = indexMapB[cityA.index];
		}
		// Add city to child
		child.route[mappedBIndex] = indB.route[i];
	}
	// Remaining unaccessed cities just get copied over
	for (int i = 0; i < indA.dimension; i++) {
		if (child.route[i].index == -1) {
			child.route[i] = indB.route[i];
		}
	}
	// Recalculate fitness
	child.calculateFitness();

	return child;
}

CycleCrossover::CycleCrossover(int dimension) : CrossoverOperator(dimension) {
}

Individual CycleCrossover::crossover(Individual& indA, Individual& indB) {
	// Reset child's indexes to -1
	resetChild();
	// If dimension too low, crossover useless
	if (indA.dimension <= 2) {
		child.route = indA.route;
		child.fitness = indA.fitness;
		return child;
	}

	unordered_map<int, int> indexAMap;
	// Map city indexes in A route to respective route indexes
	for (int i = 0; i < indA.dimension; i++) {
		indexAMap[indA.route[i].index] = i;
	}

	int cycle = 0;
	int lastCycleStart = 0;
	int idx = 0;
	set<int> indexesVisited;

	while (indexesVisited.size() != indA.dimension) {
		// Find first unvisited index (looks ugly but more efficient than it looks)
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
		// Cycle through indexes and cities of parents and assign to child
		while (indexesVisited.find(idx) == indexesVisited.end()) {
			child.route[idx] = cycle % 2 == 0 ? indA.route[idx] : indB.route[idx];

			indexesVisited.insert(idx);

			idx = indexAMap[indB.route[idx].index];
		}
		// Increment cycle counter
		cycle += 1;
	}
	// Recalculate fitness
	child.calculateFitness();

	return child;
}