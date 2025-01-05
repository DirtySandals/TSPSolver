#pragma once
#ifndef RANDOMUTIL_H
#define RANDOMUTIL_H

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>

using namespace std;

namespace RandomUtil {
	uint64_t getRandomInt(int min, int max) {
		random_device rd;
		default_random_engine eng(rd());
		uniform_int_distribution<int> distrDimension(min, max);

		return distrDimension(eng);
	}

	void sampleTwo(int dimension, int samples[2]) {
		samples[0] = getRandomInt(0, dimension - 1);
		samples[1] = samples[0];

		while (samples[1] != samples[0]) {
			samples[1] = getRandomInt(0, dimension - 1);
		}
	}

	void shuffleVector(vector<City>& list, int startIndex, int endIndex) {
		random_device rd;
		default_random_engine eng(rd());

		shuffle(list.begin() + startIndex, list.begin() + endIndex, eng);
	}
}

#endif // !RANDOMUTIL_H
