#include "InverOver.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>
#include <fstream>

using namespace std;

InverOver::InverOver() {
}

void InverOver::invert(size_t indexA, size_t indexB, std::vector<City>& solution) {
	size_t temp;
	if (indexB < indexA) {
		temp = indexA;
		indexA = indexB;
		indexB = temp;
	}
	std::reverse(solution.begin() + indexA + 1, solution.begin() + indexB + 1);
}

void InverOver::runGeneration() {
	random_device rd;
	default_random_engine eng(rd());
	uniform_real_distribution<float> distrFloat(0.0, 1.0);
	uniform_int_distribution<int> distrDimension(0, dimension - 1);
	uniform_int_distribution<int> distrPop(0, population_size - 1);
	for (int k = 0; k < population_size; k++) {
		for (int i = 0; i < dimension; i++) {
			currSol[i] = population->population[k].route[i];
		}
		mt19937 g(rd());
		shuffle(order.begin(), order.end(), g);

		for (int j = 0; j < dimension; j++) {
			mt19937 eng(rd());
			size_t i = order[j];
			City* cityA = &currSol[i];
			City* cityB;
			size_t indexB = -1000;
			if (distrFloat(eng) < 0.02f) {
				int randIndex = i;
				while (randIndex == i) randIndex = distrDimension(eng);
				cityB = &currSol[randIndex];
				indexB = randIndex;
			} else {
				int randIndex = k;
				while (randIndex == k) randIndex = distrPop(eng);
				Individual* anotherIndividual = &population->population[randIndex];

				size_t anotherCityAIndex = anotherIndividual->index(cityA);
				cityB = &anotherIndividual->route[(anotherCityAIndex + 1) % dimension];
			}
			City* previousCity = &currSol[(i - 1) % dimension];
			City* nextCity = &currSol[(i + 1) % dimension];
			if (cityB->index == previousCity->index || cityB->index == nextCity->index) break;
			for (int v = 0; v < dimension; v++) {
				if (currSol[v].index == cityB->index) {
					indexB = v;
				}
			}
			invert(i, indexB, currSol);
		}
		float currFitness = Individual::calculateFitness(currSol, dimension);

		if (currFitness < population->population[k].fitness) {
			for (int i = 0; i < dimension; i++) {
				population->population[k].route[i] = currSol[i];
			}
			population->population[k].fitness = currFitness;
		}
	}
	population->calculateAllFitness();
}
