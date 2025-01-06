#include <iostream>
#include "IGA.h"
#include "TSPProblem.h"
#include "GeneticAlgorithm.h"
#include "SelectionOperators.h"
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

using namespace std;

int main() {
	int populationSize = 50;
	MutationOperator* mutator = new SwapMutation();
	CrossoverOperator* crossover = new PMXCrossover(populationSize);
	SelectionOperator* selector = new FitnessProportionalSelection(populationSize);

	vector<string> fileNames = { "a280.txt", "pcb442.txt", "pr2392.txt", "usa13509.txt" }; //"eil51.txt" };//, "st70.txt", "eil76.txt", "kroA100.txt", "kroC100.txt", "eil101.txt", "lin105.txt", "a280.txt", "pcb442.txt", "pr2392.txt", "usa13509.txt"};
	for (int i = 0; i < 30; i++) {
		for (string fileName : fileNames) {
			cout << "Running " << fileName << "..." << endl;
			TSPProblem problem;
			problem.constructProblem(fileName);
			problem.printCities();
			GeneticAlgorithm ga = GeneticAlgorithm(problem, populationSize, mutator, crossover, selector);
			float fittest = ga.startGA();

			fileName.erase(fileName.size() - 4);
			string outname = fileName + "_best_generations.txt";
			ofstream file(outname, ios::app);
			if (!file.is_open()) continue;
			file << to_string(fittest) + '\n';
			file.close();
		}
	}

	IGA iga = IGA();
	string command = "";

	while (command != "quit") {
		getline(cin, command);
		iga.processCommand(command);
	}

	return 0;
}