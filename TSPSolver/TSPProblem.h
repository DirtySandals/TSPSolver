#pragma once

#ifndef TSPPROBLEM_H
#define TSPPROBLEM_H

#include "City.h"
#include <fstream>
#include <vector>
#include <string>

class TSPProblem {
public:
	std::vector<City> problem;
	int dimension;
	TSPProblem();
	void constructProblem(std::string file);
	void readCoordinates(std::vector<std::string>& lines, int startIndex);
	void printCities();
};

#endif // !TSPPROBLEM_H