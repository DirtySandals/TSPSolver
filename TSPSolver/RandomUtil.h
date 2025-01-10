#pragma once
#ifndef RANDOMUTIL_H
#define RANDOMUTIL_H

#include "Individual.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>

using namespace std;

// RandomUtil is namespace for a collection of functions used throughout the program
namespace RandomUtil {
	// Returns uniformly random integer in range [min, max]
	inline int getRandomInt(int min, int max) {
		// Initialise random number generator
		random_device rd;
		default_random_engine eng(rd());
		uniform_int_distribution<int> distrDimension(min, max);

		return distrDimension(eng);
	}
	// Populates array with two unique random integers in range [0, dimension)
	inline void sampleTwo(int dimension, int samples[2]) {
		samples[0] = getRandomInt(0, dimension - 1);
		samples[1] = samples[0];
		// Get second random integer that is not equal to first sampled integer
		while (samples[1] == samples[0]) {
			samples[1] = getRandomInt(0, dimension - 1);
		}
	}
	// Populates array with numSamples number of random integers in range [0, dimension)
	inline void sample(int dimension, int samples[], int numSamples) {
		if (numSamples == 0) {
			return;
		}
		// Initialise random number generator
		random_device rd;
		default_random_engine eng(rd());
		uniform_int_distribution<int> distrDimension(0, dimension - 1);
		// Get first sample
		samples[0] = distrDimension(eng);
		int idx = 1;
		samples[1] = -1;
		// Sample the next samples that have not been sampled yet
		// This is inefficient but I think on average it beats shuffling an array/vector
		// to sample
		while (idx != numSamples) {
			samples[idx] = distrDimension(eng);
			bool isDuplicate = false;
			// Check past samples for uniqueness
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
	// Shuffles list vector from startIndex to endIndex
	inline void shuffleVector(vector<City>& list, int startIndex, int endIndex) {
		// Initialise random number generator
		random_device rd;
		default_random_engine eng(rd());
		// Shuffle vector
		shuffle(list.begin() + startIndex, list.begin() + endIndex + 1, eng);
	}
	// Populates individuals parameter with individuals from population via discrete random distribution
	inline void weightDistribution(vector<Individual>& population, vector<float>& distribution, Individual individuals[]) {
		// Initialise weighted discrete distribution
		random_device rd;
		default_random_engine eng(rd());
		discrete_distribution<> wDist(distribution.begin(), distribution.end());

		// Sample from weighted distribution and assign individuals an individual from population
		for (int i = 0; i < distribution.size(); i++) {
			int rdmIdx = wDist(eng);
			individuals[i] = move(population[rdmIdx]);
		}
	}
}

#endif // !RANDOMUTIL_H
