#pragma once
#ifndef STATTRACKER_H
#define STATTRACKER_H

#include "City.h"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

struct StatTracker {
	vector<City> bestRoute;
	float bestFitness = numeric_limits<float>::max();
	int bestGeneration = -1;

	int currentGeneration = 0;
	float currentBestFitness = numeric_limits<float>::max();
	float currentWorstFitness = numeric_limits<float>::min();

	StatTracker() {

	}

	StatTracker(int dimension) {
		bestRoute.reserve(dimension);

		for (int i = 0; i < dimension; i++) {
			bestRoute.push_back(City(-1, 0, 0));
		}
	}

	void update(vector<City>& route, float fitness) {
		currentWorstFitness = max(fitness, currentWorstFitness);
		currentBestFitness = min(fitness, currentBestFitness);

		if (fitness >= bestFitness) {
			return;
		}

		bestFitness = fitness;

		bestGeneration = currentGeneration;

		cout << "bestroute: ";
		for (int i = 0; i < bestRoute.size(); i++) {
			if (bestRoute[i].index != route[i].index) {
				bestRoute[i] = move(route[i]);
			}
			cout << to_string(bestRoute[i].index) << " ";
		}
		cout << endl;
	}

	void updateGen() {
		currentGeneration++;
		currentBestFitness = numeric_limits<float>::max();
		currentWorstFitness = numeric_limits<float>::min();
	}
};

#endif // !STATTRACKER_H
