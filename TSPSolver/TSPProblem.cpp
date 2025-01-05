#include "TSPProblem.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
using namespace std;

TSPProblem::TSPProblem() {

}

void TSPProblem::constructProblem(std::string filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		throw std::out_of_range("file not found");
	}
	string line;
	vector<string> lines;
	lines.reserve(13509);
	while (getline(file, line)) lines.push_back(line);
	int end = lines.size();
	for (int i = 0; i < end; i++) {
		istringstream iss(lines[i]);
		vector<string> words;
		string word;
		while (iss >> word) words.push_back(word);
		if (words[0] == "DIMENSION" || words[0] == "DIMENSION:") {
			dimension = stoi(words[words.size() - 1]);
		} else if (words[0] == "NODE_COORD_SECTION") {
			readCoordinates(lines, i + 1);
			break;
		}
	}
	file.close();
}

void TSPProblem::readCoordinates(std::vector<std::string>& lines, int startIndex) {
	int end = lines.size();
	for (int i = startIndex; i < end; i++) {
		if (lines[i] == "EOF") break;
		istringstream iss(lines[i]);
		vector<string> words;
		string word;
		while (iss >> word) words.push_back(word);
		if (words.size() < 3) break;
		problem.push_back(City(stoi(words[0]), stof(words[1]), stof(words[2])));
	}
}

void TSPProblem::printCities() {
	for (City city : problem) {
		cout << city.index << ", " << to_string(city.x) << ", " << to_string(city.y) << endl;
	}
}
