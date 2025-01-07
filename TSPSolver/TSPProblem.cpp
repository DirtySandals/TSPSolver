#include "TSPProblem.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

TSPProblem::TSPProblem() {

}

void TSPProblem::parseFile(std::string filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("instance file not found");
	}
	string line;
	vector<string> lines;

	while (getline(file, line)) 
		lines.push_back(line);
	
	parseInstance(lines);

	file.close();
}

void TSPProblem::parseInstance(std::vector<std::string>& lines) {
	int end = lines.size();

	if (lines.size() == 0) {
		throw out_of_range("cannot load a problem of 0 entries");
	}

	for (int i = 0; i < end; i++) {
		istringstream iss(lines[i]);
		vector<string> words;
		string word;
		while (iss >> word) 
			words.push_back(word);

		if (words[0] == "NODE_COORD_SECTION") {
			readCoordinates(lines, i + 1);
			dimension = problem.size();
			break;
		}
	}

	if (problem.size() == 0) {
		throw runtime_error("instance contained no entries");
	}
}

void TSPProblem::readCoordinates(std::vector<std::string>& lines, int startIndex) {
	int end = lines.size();

	if (end > 0) {
		problem.clear();
	}

	for (int i = startIndex; i < end; i++) {
		if (lines[i] == "EOF") 
			break;

		istringstream iss(lines[i]);
		vector<string> words;
		string word;

		while (iss >> word) 
			words.push_back(word);

		if (words.size() < 3) 
			break;

		problem.push_back(City(stoi(words[0]), stof(words[1]), stof(words[2])));
	}
}

void TSPProblem::printCities() {
	for (City city : problem) {
		cout << city.index << ", " << to_string(city.x) << ", " << to_string(city.y) << endl;
	}
}
