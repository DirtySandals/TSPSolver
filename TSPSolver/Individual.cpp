#include "Individual.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <random> 
#include <unordered_map>

using namespace std;

float Individual::calculateDistance(City& city1, City& city2) {
	// Calculate euclidean distance between two points
	float squareSum = pow(city2.x - city1.x, float(2)) + pow(city2.y - city1.y, float(2));
	return sqrt(squareSum);
}

Individual::Individual() {

}

Individual::Individual(std::vector<City> problem) {
	// Initialise variables
	fitness = numeric_limits<float>::max();
	dimension = problem.size();
	route.reserve(dimension);
	// Copy problem
	for (int i = 0; i < dimension; i++) {
		route.push_back(problem[i]);
	}
	// Randomise the route
	randomize();
}

void Individual::randomize() {
	// Set up random number generator
	random_device rd;
	mt19937 g(rd());
	// Randomise route
	shuffle(route.begin(), route.end(), g);
}

float Individual::calculateFitness() {
	// Calculate distance between all cities in route (including start to end) to create hamiltonian path
	float totalDistance = 0;
	for (int i = 0; i < dimension - 1; i++) {
		totalDistance += calculateDistance(route[i], route[i + 1]);
	}
	totalDistance += calculateDistance(route[0], route[dimension - 1]);
	fitness = totalDistance;

	return fitness;
}

size_t Individual::index(City* city) {
	// Search route for city index
	for (int i = 0; i < dimension; i++) {
		if (route[i].index == city->index) 
			return i;
	}
	// If not found, create error message
	unordered_map<int, int> nums;
	for (int i = 0; i < dimension; i++) {
		nums[i] += 1;
	}
	for (int i = 0; i < dimension; i++) {
		if (nums[i] == 0) {
			cout << "index " << i << " doesnt exist" << endl;
		} else if (nums[i] > 1) {
			cout << "index " << i << " has " << nums[i] << " duplicates" << endl;
		}
	}
	throw out_of_range("yo");

}

float Individual::calculateFitness(std::vector<City>& solution) {
	// Calculate euclidean distance across hamiltonian path of external solution
	float totalDistance = 0;
	int dimension = solution.size();
	for (int i = 0; i < dimension - 1; i++) {
		totalDistance += calculateDistance(solution[i], solution[i + 1]);
	}
	totalDistance += calculateDistance(solution[0], solution[dimension - 1]);

	return totalDistance;
}

std::string Individual::toString() {
	// Convert route to string for printing purposes
	string output = "Route:\n{";
	for (int i = 0; i < dimension; i++) {
		output += to_string(route[i].index);
		if (i != dimension - 1) {
			output += ", ";
		}
	}

	output += "}";

	return output;
}

void Individual::operator=(const Individual& other) {
	// Copy over member variables
	this->route = other.route;
	this->dimension = other.dimension;
	this->fitness = other.fitness;
}

std::ostream& operator<<(std::ostream& os, const Individual& ind) {
	// Print string version of route
	string output = "Route:\n{";
	for (int i = 0; i < ind.dimension; i++) {
		output += to_string(ind.route[i].index);
		if (i != ind.dimension - 1) {
			output += ", ";
		}
	}

	output += "}";

	os << output << endl;
	return os;
}
