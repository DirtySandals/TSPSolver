#pragma once
#ifndef RANDOMUTIL_H
#define RANDOMUTIL_H

#include "Individual.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>

using namespace std;

namespace RandomUtil {
	inline int getRandomInt(int min, int max) {
		random_device rd;
		default_random_engine eng(rd());
		uniform_int_distribution<int> distrDimension(min, max);

		return distrDimension(eng);
	}

	inline void sampleTwo(int dimension, int samples[2]) {
		samples[0] = getRandomInt(0, dimension - 1);
		samples[1] = samples[0];

		while (samples[1] != samples[0]) {
			samples[1] = getRandomInt(0, dimension - 1);
		}
	}

	inline void sample(int dimension, int samples[], int numSamples) {
		if (numSamples == 0) {
			return;
		}

		random_device rd;
		default_random_engine eng(rd());
		uniform_int_distribution<int> distrDimension(0, dimension - 1);

		samples[0] = distrDimension(eng);
		int idx = 1;
		samples[1] = -1;
		while (idx != numSamples) {
			samples[idx] = distrDimension(eng);
			bool isDuplicate = false;
			for (int i = 0; i < idx; i++) {
				if (samples[idx] == samples[i]) {
					isDuplicate = true;
					break;
				}
			}
			if (isDuplicate) {
				continue;
			}
			idx++;
		}

	}

	inline void shuffleVector(vector<City>& list, int startIndex, int endIndex) {
		random_device rd;
		default_random_engine eng(rd());

		shuffle(list.begin() + startIndex, list.begin() + endIndex + 1, eng);
	}

	inline void weightDistribution(vector<Individual>& population, vector<float>& distribution, Individual individuals[]) {
		
		random_device rd;
		default_random_engine eng(rd());
		discrete_distribution<> wDist(distribution.begin(), distribution.end());

		
		for (int i = 0; i < distribution.size(); i++) {
			int rdmIdx = wDist(eng);
			individuals[i] = move(population[rdmIdx]);
		}
	}
}

#endif // !RANDOMUTIL_H
