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

InverOver::InverOver(TSPProblem& tspProblem, int populationSize) : GeneticAlgorithm(tspProblem, populationSize, nullptr, nullptr, nullptr) {
	// Initialise vectors to reduce memory allocation
	currSol.reserve(dimension);
	order.reserve(dimension);

	for (int i = 0; i < tspProblem.dimension; i++) {
		currSol.push_back(City());
		order.push_back(i);
	}
}

void InverOver::invert(size_t indexA, size_t indexB, std::vector<City>& solution) {
	// sort indexes
	if (indexB < indexA) {
		swap(indexA, indexB);
	}
	// Reverse segment
	std::reverse(solution.begin() + indexA + 1, solution.begin() + indexB + 1);
}


/*
 * Inverover iterates over each invdividual, finds two random cities, and reverses the segment
 * at end between the cities. If the reversed segment is beneficial, the solution is kept, otherwise,
 * the individual is untouched
 */
void InverOver::runGeneration() {
	// Init random distributions
	random_device rd;
	default_random_engine eng(rd());
	uniform_real_distribution<float> distrFloat(0.0, 1.0);
	uniform_int_distribution<int> distrDimension(0, dimension - 1);
	uniform_int_distribution<int> distrPop(0, populationSize - 1);
	// For each individual
	for (int k = 0; k < populationSize; k++) {
		// Copy individual solution
		for (int i = 0; i < dimension; i++) {
			currSol[i] = population->population[k].route[i];
		}
		// Shuffle order of indexes to visit
		mt19937 g(rd());
		shuffle(order.begin(), order.end(), g);

		for (int j = 0; j < dimension; j++) {
			mt19937 eng(rd());
			size_t i = order[j];
			City* cityA = &currSol[i];
			City* cityB;
			size_t indexB = -1000;
			// If 2% chance, assign CityB to random city that is not cityA
			if (distrFloat(eng) < 0.02f) {
				// Get random index thats not i
				int randIndex = i;
				while (randIndex == i) 
					randIndex = distrDimension(eng);
				// Assign cityB to randomly found city
				cityB = &currSol[randIndex];
				indexB = randIndex;
			} 
			// If 98% chance, assign cityB the index in another individual
			else {
				// Select another individual
				int randIndex = k;
				while (randIndex == k) 
					randIndex = distrPop(eng);
				Individual* anotherIndividual = &population->population[randIndex];
				// Find index of cityA in other individual
				size_t anotherCityAIndex = anotherIndividual->index(cityA);
				// CityB is proceeding city in other individual
				cityB = &anotherIndividual->route[(anotherCityAIndex + 1) % dimension];
			}
			// If cityB is either pevious or next, iterate to next city
			City* previousCity = &currSol[(i - 1) % dimension];
			City* nextCity = &currSol[(i + 1) % dimension];

			if (cityB->index == previousCity->index || cityB->index == nextCity->index) 
				break;
			// Find index of cityB
			for (int v = 0; v < dimension; v++) {
				if (currSol[v].index == cityB->index) {
					indexB = v;
				}
			}
			// Reverse segment at and between cityA and cityB
			invert(i, indexB, currSol);
		}
		// Recalculate fitness of individual
		float currFitness = Individual::calculateFitness(currSol);
		// If found solution is better than current individual fitness, replace individual's solution
		if (currFitness < population->population[k].fitness) {
			for (int i = 0; i < dimension; i++) {
				population->population[k].route[i] = currSol[i];
			}
			population->population[k].fitness = currFitness;
		}
	}

	// Recalculate all fitnesses
	population->calculateAllFitness();
}
