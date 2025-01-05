#include "Individual.h"
#include <iostream>
#include <random> 
#include <algorithm>
#include <cmath>
#include <unordered_map>

using namespace std;

float Individual::calculateDistance(City& city1, City& city2) {
	float squareSum = pow(city2.x - city1.x, float(2)) + pow(city2.y - city1.y, float(2));
	return sqrt(squareSum);
}

Individual::Individual() {

}

Individual::Individual(int dimension, std::vector<City> problem) : dimension(dimension) {
	route.reserve(dimension);
	routeMap.reserve(dimension);
	for (int i = 0; i < dimension; i++) routeMap.push_back(0);
	for (int i = 0; i < dimension; i++) {
		route.push_back(problem[i]);
		if (problem[i].index - 1 >= dimension) {
			cout << "out of range" << endl;
			cout << problem[i].index << endl;
		}
		routeMap[problem[i].index - 1] = i;
	}
	randomize();
}

void Individual::randomize() {
	random_device rd;
	mt19937 g(rd());
	shuffle(route.begin(), route.end(), g);
	for (int i = 0; i < dimension; i++) {
		routeMap[route[i].index - 1] = i;
	}
}



Individual::~Individual() {
}

void Individual::loadRoute(City problem[]) {
	for (int i = 0; i < dimension; i++) {
		route[i] = problem[i];
	}
	//randomize();
	//calculateFitness();
}

int Individual::size() {
	return route.size();
}

float Individual::calculateFitness() {
	float totalDistance = 0;
	for (int i = 0; i < dimension - 1; i++) {
		totalDistance += calculateDistance(route[i], route[i + 1]);
	}
	totalDistance += calculateDistance(route[0], route[dimension - 1]);
	fitness = totalDistance;
	return fitness;
}

size_t Individual::index(City* city) {
	//cout << dimension << endl;
	//cout << route.size() << endl;
	for (int i = 0; i < dimension; i++) {
		if (route[i].index == city->index) return i;
	}
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

float Individual::calculateFitness(std::vector<City>& solution, int dimension) {
	float totalDistance = 0;
	for (int i = 0; i < dimension - 1; i++) {
		totalDistance += calculateDistance(solution[i], solution[i + 1]);
	}
	totalDistance += calculateDistance(solution[0], solution[dimension - 1]);
	/*
	cout << "total distance: " << totalDistance << endl;
	cout << dimension << endl;
	for (int i = 0; i < dimension; i++) {
		cout << solution[i].index << ", ";
	}
	cout << endl;
	*/
	return totalDistance;
}