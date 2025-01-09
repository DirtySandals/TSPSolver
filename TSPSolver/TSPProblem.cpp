#include "TSPProblem.h"

#include "StringUtil.h"

#include <stdexcept>
#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

TSPProblem::TSPProblem() {

}

void TSPProblem::parseFile(std::string filename) {
	cout << 1 << endl;
	ifstream file(filename);
	try {
		if (!file.is_open()) {
			cout << "couldnt open" << endl;
			throw std::runtime_error("instance file not found");
		}
		cout << 2 << endl;
		string line;
		vector<string> lines;
		cout << 3 << endl;
		while (getline(file, line))
			lines.push_back(line);
		cout << 4 << endl;
		parseInstance(lines);

		file.close();
	} catch (exception& e) {
		cerr << e.what() << endl;
		file.close();
	}
}

void TSPProblem::parseInstance(std::vector<std::string>& lines) {
	cout << 2 << endl;
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

void TSPProblem::parseCustomInstance(std::vector<std::string>& lines) {
	try {
		int end = lines.size();

		if (lines.size() == 0) {
			throw out_of_range("cannot load a problem of 0 entries");
		}

		for (int i = 0; i < lines.size(); i++) {
			if (StringUtil::trim(lines[i]) == "EOF") {
				break;
			}

			vector<string> numbers = StringUtil::split(lines[i], ',');

			if (numbers.size() != 2) {
				throw runtime_error("invalid coordinate: " + lines[i]);
			}

			numbers[0] = StringUtil::trim(numbers[0]);
			numbers[1] = StringUtil::trim(numbers[1]);

			int x_coord = static_cast<int>(stod(numbers[0]));
			int y_coord = static_cast<int>(stod(numbers[1]));

			problem.push_back(City(i + 1, x_coord, y_coord));
			dimension = problem.size();
		}

		if (problem.size() == 0) {
			throw runtime_error("instance contained no entries");
		}
	} catch (exception& e) {
		cerr << e.what() << endl;
		dimension = 0;
		problem.clear();
	}
}

void TSPProblem::readCoordinates(std::vector<std::string>& lines, int startIndex) {
	cout << 3 << endl;
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
