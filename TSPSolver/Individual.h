#pragma once

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "City.h"

#include <string>
#include <vector>

// Individual abstracts solution of TSP problem
class Individual {
	private:
		// Calculates distance between two 2D points (cities)
		static float calculateDistance(City& city1, City& city2);
	public:
		// Static function calculates fitness of an arbitrary route
		static float calculateFitness(std::vector<City>& solution);

		std::vector<City> route; // Permuation route of problem
		int dimension; // Length of route
		float fitness = -1; // Fitness (distance of route)

		Individual();
		Individual(std::vector<City> problem);
		// Randomises the route
		void randomize();
		// Calculates the fitness of the individual
		float calculateFitness();
		// Returns the index of a city in the route
		size_t index(City* city);
		// Returns the route in form of string
		std::string toString();
		// Overridden equals operator for faster assigning
		void operator=(const Individual& other);
		// Friend function for printing individual
		friend std::ostream& operator<<(std::ostream& os, const Individual& ind);
};

#endif // !INDIVIDUAL_H