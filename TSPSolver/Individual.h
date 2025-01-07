#pragma once

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "City.h"

#include <string>
#include <vector>

class Individual {
private:
	static float calculateDistance(City& city1, City& city2);
	std::vector<size_t> routeMap;
public:
	std::vector<City> route;
	int dimension;
	float fitness = -1;
	Individual();
	Individual(std::vector<City> problem);
	~Individual();
	void randomize();
	void loadRoute(City problem[]);
	int size();
	float calculateFitness();
	size_t index(City* city);
	static float calculateFitness(std::vector<City>& solution, int dimension);
	void operator[](size_t index);
	std::string toString();
	void operator=(const Individual& other);
	friend std::ostream& operator<<(std::ostream& os, const Individual& ind);
};

#endif // !INDIVIDUAL_H