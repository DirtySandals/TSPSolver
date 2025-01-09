#pragma once

#ifndef TSPPROBLEM_H
#define TSPPROBLEM_H

#include "City.h"
#include <fstream>
#include <vector>
#include <string>

class TSPProblem {
private:
	void readCoordinates(std::vector<std::string>& lines, int startIndex);
public:
	std::vector<City> problem;
	int dimension = 0;
	TSPProblem();
	void parseFile(std::string file);
	void parseInstance(std::vector<std::string>& lines);
	void parseCustomInstance(std::vector<std::string>& lines);
	void printCities();
};

#endif // !TSPPROBLEM_H