#pragma once
#ifndef STATTRACKER_H
#define STATTRACKER_H

#include "City.h"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

// StatTracker is a struct which holds the meaningful stats of the genetic algorithm
struct StatTracker {
	vector<City> bestRoute; // Route with best fitness found globally
	float bestFitness = numeric_limits<float>::max(); // Fitness associated with best route
	int bestGeneration = -1; // Generation best route was found in
	// Stats for current generation
	int currentGeneration = 0;
	float currentBestFitness = numeric_limits<float>::max();
	float currentWorstFitness = numeric_limits<float>::min();

	// Constructors
	StatTracker() {

	}

	StatTracker(int dimension) {
		bestRoute.reserve(dimension);

		for (int i = 0; i < dimension; i++) {
			bestRoute.push_back(City(-1, 0, 0));
		}
	}
	// Updates stats
	void update(vector<City>& route, float fitness) {
		currentWorstFitness = max(fitness, currentWorstFitness);
		currentBestFitness = min(fitness, currentBestFitness);

		if (fitness >= bestFitness) {
			return;
		}

		bestFitness = fitness;

		bestGeneration = currentGeneration;

		// Prints the best route when found
		cout << "bestroute: ";
		for (int i = 0; i < bestRoute.size(); i++) {
			if (bestRoute[i].index != route[i].index) {
				bestRoute[i] = move(route[i]);
			}
			cout << to_string(bestRoute[i].index) << " ";
		}
		cout << endl;
	}
	// Updates current generation variables by 'resetting' them
	void updateGen() {
		currentGeneration++;
		currentBestFitness = numeric_limits<float>::max();
		currentWorstFitness = numeric_limits<float>::min();
	}
};

#endif // !STATTRACKER_H
