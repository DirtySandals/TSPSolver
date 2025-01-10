#pragma once

#ifndef TSPPROBLEM_H
#define TSPPROBLEM_H

#include "City.h"
#include <fstream>
#include <vector>
#include <string>

// TSPProblem represents the TSP instance
class TSPProblem {
	private:
		// Reads the coordinates from vector of strings from a beginning index
		void readCoordinates(std::vector<std::string>& lines, int startIndex);
	public:
		std::vector<City> problem; // Lists all cities in problem
		int dimension = 0; // The size of problem
		// Constructor
		TSPProblem();
		// Parses typical TSP instance file and creates problem vector
		void parseFile(std::string file);
		// Parse instance from TSP instance file
		void parseInstance(std::vector<std::string>& lines);
		// Parse custom instance from terminal
		void parseCustomInstance(std::vector<std::string>& lines);
		// Print all cities in problem
		void printCities();
};

#endif // !TSPPROBLEM_H