#pragma once
#ifndef IGA_H
#define IGA_H

#include "GeneticAlgorithm.h"
#include "TSPProblem.h" 

#include <string>
#include <thread>
#include <vector>

class IGA {
	private:
		static const std::vector<std::string> fileNames;
		static const std::vector<std::string> loadLabels;
		static const std::vector<std::string> startLabels;
		static const std::string author;
		static const std::string programName;
		static const int tournamentSize = 5;

		std::thread gaThread;

		GeneticAlgorithm* ga = nullptr;
		TSPProblem* tspProblem = nullptr;

		int getLabelledValueInt(std::string text, std::string label, const std::vector<std::string> allLabels);
		std::string getLabelledValue(std::string text, std::string label, const std::vector<std::string> allLabels);

		void selectMutation(MutationOperator* mutator, std::string name);
		void selectCrossover(CrossoverOperator* crossover, std::string name, int dimension);
		void selectSelection(SelectionOperator* selector, std::string name, int populationSize);
	public:
		IGA();
		~IGA();
		void processCommand(std::string command);
		void processLoad(std::string command);
		void processStart(std::string command);
		void stop();
		void quit();
		void help();
};

#endif // !IGA_H
