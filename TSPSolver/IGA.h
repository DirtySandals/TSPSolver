#pragma once
#ifndef IGA_H
#define IGA_H

#include "GeneticAlgorithm.h"
#include "TSPProblem.h" 

#include <string>
#include <thread>
#include <vector>

// Interface for program, gives interactivity to genetic algorithm
class IGA {
	private:
		// Keyword labels for different commands
		static const std::vector<std::string> fileNames;
		static const std::vector<std::string> loadLabels;
		static const std::vector<std::string> startLabels;
		// Const information
		static const std::string author;
		static const std::string programName;
		static const int tournamentSize = 5;

		// Thread for running the genetic algorithm
		std::thread gaThread;

		GeneticAlgorithm* ga = nullptr;
		TSPProblem* tspProblem = nullptr;

		// Functions used for attaining value following input label
		int getLabelledValueInt(std::string text, std::string label, const std::vector<std::string> allLabels);
		std::string getLabelledValue(std::string text, std::string label, const std::vector<std::string> allLabels);

		// Functions returning operator associated with input name
		MutationOperator* selectMutation(std::string name);
		CrossoverOperator* selectCrossover(std::string name, int dimension);
		SelectionOperator* selectSelection(std::string name, int populationSize);

		// Process request functions
		void processLoad(std::string command);
		void processStart(std::string command);
		void stop();
		void quit();
		void help();
	public:
		IGA();
		~IGA();
		// Process user input
		void processCommand(std::string command);
};

#endif // !IGA_H
