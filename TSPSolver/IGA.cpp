#include "IGA.h"

#include "InverOver.h"
#include "StringUtil.h"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;
using namespace StringUtil;

const vector<string> IGA::fileNames = { "a280", "eil51", "eil76", "eil101", "kroA100", "kroC100", "lin105", "pcb442", "pr2392", "st70", "usa13509" };
const vector<string> IGA::loadLabels = { "load", "file", "instance" };
const vector<string> IGA::startLabels = { "start", "maxgenerations", "populationsize", "mutation", "crossover", "selection" };
const string IGA::author = "Aleksander Gayko";
const string IGA::programName = "TSPSolver";

IGA::IGA() {
	tspProblem = new TSPProblem();

	cout << programName << " by " << author << endl;
	cout << "type: 'help' for help" << endl;
}

void IGA::selectMutation(MutationOperator* mutator, std::string name) {
	name = toLower(name);

	if (name == "insert") {
		mutator = new InsertMutation();
	} else if (name == "swap") {
		mutator = new SwapMutation();
	} else if (name == "inversion") {
		mutator = new InversionMutation();
	} else if (name == "scramble") {
		mutator = new ScrambleMutation();
	} else {
		throw new invalid_argument(name + " is not a valid mutation operator");
	}
}

void IGA::selectCrossover(CrossoverOperator* crossover, std::string name, int dimension) {
	name = toLower(name);

	if (name == "order") {
		crossover = new OrderCrossover(dimension);
	} else if (name == "pmx") {
		crossover = new PMXCrossover(dimension);
	} else if (name == "cycle") {
		crossover = new CycleCrossover(dimension);
	} else {
		throw new invalid_argument(name + " is not a valid crossover operator");
	}
}

void IGA::selectSelection(SelectionOperator* selector, std::string name, int populationSize) {
	name = toLower(name);

	if (name == "fitness") {
		selector = new FitnessProportionalSelection(populationSize);
	} else if (name == "tournament") {
		selector = new TournamentSelection(tournamentSize);
	} else if (name == "elitism") {
		selector = new ElitismSelection();
	} else {
		throw new invalid_argument(name + " is not a valid selection operator");
	}
}

IGA::~IGA() {
	delete ga;
	delete tspProblem;
}

void IGA::processCommand(string command) {
	try {
		command = trim(command);
		const string commandType = toLower(splitString(command)[0]);

		if (commandType == "load") {
			processLoad(command);
		} else if (commandType == "start") {
			processStart(command);
		} else if (commandType == "stop") {
			stop();
		} else if (commandType == "quit") {
			quit();
		} else if (commandType == "help") {
			help();
		}
	} catch (exception& e) {
		cerr << e.what() << endl;
	}
}

void IGA::processLoad(std::string command) {
	if (contains(command, "file")) {
		string fileName = getLabelledValue(command, "file", loadLabels);
		fileName += ".txt";

		tspProblem->parseFile(fileName);

		delete ga;
		ga = nullptr;
	} else if (contains(command, "instance")) {
		string instance = getLabelledValue(command, "instance", loadLabels);

		vector<string> lines = split(instance, '\n');

		tspProblem->parseInstance(lines);

		delete ga;
		ga = nullptr;
	}
}

void IGA::processStart(std::string command) {
	if (gaThread.joinable()) {
		throw new runtime_error("genetic algorithm still running...");
	}

	if (tspProblem->dimension == 0) {
		throw new invalid_argument("problem has not been loaded");
	}

	int maxGenerations = 20000;
	int populationSize = 50;

	MutationOperator* mutator = nullptr;
	CrossoverOperator* crossover = nullptr;
	SelectionOperator* selector = nullptr;

	if (contains(command, "maxgenerations")) {
		int inputMaxGens = getLabelledValueInt(command, "maxgenerations", startLabels);

		if (inputMaxGens < 1000) {
			throw new out_of_range("maxgenerations must be greater than 1000");
		}

		maxGenerations = inputMaxGens;
	}

	if (contains(command, "populationsize")) {
		int inputPopSize = getLabelledValueInt(command, "populationSize", startLabels);

		if (inputPopSize <= 0 || inputPopSize >= 100000) {
			throw new out_of_range("populationsize must be greater than 1 and less than 100,000");
		}

		populationSize = inputPopSize;
	}

	if (contains(command, "inverover")) {
		ga = new InverOver(*tspProblem, populationSize);
		gaThread = thread(&GeneticAlgorithm::startGA, ga, maxGenerations);
		return;
	}

	if (contains(command, "mutation")) {
		string mutationInput = getLabelledValue(command, "mutation", startLabels);
		selectMutation(mutator, mutationInput);
	} else {
		mutator = new InversionMutation();
	}

	if (contains(command, "crossover")) {
		string crossoverInput = getLabelledValue(command, "crossover", startLabels);
		selectCrossover(crossover, crossoverInput, tspProblem->dimension);
	} else {
		crossover = new PMXCrossover(tspProblem->dimension);
	}

	if (contains(command, "selection")) {
		string selectionInput = getLabelledValue(command, "selection", startLabels);
		selectSelection(selector, selectionInput, populationSize);
	} else {
		selector = new TournamentSelection(tournamentSize);
	}

	ga = new GeneticAlgorithm(*tspProblem, populationSize, mutator, crossover, selector);
	gaThread = thread(&GeneticAlgorithm::startGA, ga, maxGenerations);
}

void IGA::stop() {
	if (ga == nullptr) {
		return;
	}
	if (!gaThread.joinable()) {
		return;
	}

	ga->stop();
	this_thread::sleep_for(chrono::milliseconds(100));
	gaThread.join();
	this_thread::sleep_for(chrono::milliseconds(100));
}

void IGA::quit() {
	stop();

	exit(0);
}

void IGA::help() {
	cout << "HELP" << endl;
	cout << endl;
	cout << "LOAD" << endl;
	cout << "To use the program, you must first load an instance via file name or a custom instance input." << endl;
	cout << "FILE NAME LOAD:" << endl;
	cout << "Input: 'load file <file_name>'" << endl;
	cout << endl;

	cout << "FILE NAMES:" << endl;
	
	for (const string& fileName : fileNames) {
		cout << "- " << fileName << '\n';
	}
	cout << endl;
	
	cout << "INSTANCE LOAD" << endl;
	cout << "Input: 'load instance <instance formatted string (lines separated by \\n)>'" << endl;

	cout << endl;

	cout << "START" << endl;
	cout << "To start the genetic algorithm, you must have loaded an instance." << endl;
	cout << "USING COMBINATION OF OPERATORS" << endl;
	cout << "Input: 'start maxgenerations <value : default 20000> populationsize <value : default 50> mutation <value : default 'inversion'> crossover <value : default 'pmx'> selection <value : default 'tournament'>" << endl;
	cout << "If an argument is not included its default value will be used." << endl;
	cout << endl;
	cout << "MUTATION OPERATORS" << endl;
	cout << "- insert\n- swap\n- inversion\n- scramble\n" << endl;
	cout << "CROSSOVER OPERATORS" << endl;
	cout << "- order\n- pmx\n- cycle\n" << endl;
	cout << "SELECTION OPERATORS" << endl;
	cout << "- fitness\n- tournament\n- elitism\n" << endl;

	cout << "USE INVEROVER ALGORITHM" << endl;
	cout << "Inver-over is an evolutionary algorithm from Guo Tao and Zbigniew Michalewicz which is based on simple inversion." << endl;
	cout << "More can be found in their paper: 'Inver-over Operator for the TSP'." << endl;
	cout << "Input: 'start inverover maxgenerations <value : default 20000> populationsize <value : default 50>'" << endl;

	cout << "STOP" << endl;
	cout << "To stop the current genetic algorithm, type: 'stop'." << endl;
	
	cout << endl;
}

int IGA::getLabelledValueInt(string text, string label, const vector<string> allLabels) {
	string valueString = getLabelledValue(text, label, allLabels);
	string resultString = splitString(valueString)[0];

	if (!isDigitString(resultString)) {
		throw runtime_error("'" + resultString + "' is not an integer.'");
	}

	int value = stoi(resultString);

	if (value < 0) {
		throw runtime_error("'" + to_string(value) + "' is not a positive integer");
	}

	return value;
}

string IGA::getLabelledValue(string text, string label, const vector<string> allLabels) {
	text = trim(text);

	if (!contains(text, label)) {
		throw runtime_error("'" + label + "' not found within '" + text + "'");
	}

	int valueStart = indexOf(text, label) + label.size();
	int valueEnd = text.size();

	for (int i = 0; i < allLabels.size(); i++) {
		if (allLabels[i] != label && contains(text, allLabels[i])) {
			int otherIDStartIndex = indexOf(text, allLabels[i]);
			if (otherIDStartIndex > valueStart && otherIDStartIndex < valueEnd) {
				valueEnd = otherIDStartIndex;
			}
		}
	}

	return trim(text.substr(valueStart, valueEnd - valueStart));
}