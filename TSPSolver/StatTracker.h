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

	StatTracker() {

	}

	StatTracker(int dimension) {
		bestRoute.reserve(dimension);

		for (int i = 0; i < dimension; i++) {
			bestRoute.push_back(City(-1, 0, 0));
		}
	}

	void update(vector<City>& route, float fitness, int generation) {
		if (fitness >= bestFitness) {
			return;
		}

		bestFitness = fitness;

		bestGeneration = generation;

		cout << "bestroute: ";
		for (int i = 0; i < bestRoute.size(); i++) {
			if (bestRoute[i].index != route[i].index) {
				bestRoute[i] = move(route[i]);
			}
			cout << to_string(bestRoute[i].index) << " ";
		}
		cout << endl;
	}
};

#endif // !STATTRACKER_H
