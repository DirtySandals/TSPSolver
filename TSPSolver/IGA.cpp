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

MutationOperator* IGA::selectMutation(std::string name) {
	MutationOperator* mutator = nullptr;

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
		throw invalid_argument(name + " is not a valid mutation operator");
	}

	return mutator;
}

CrossoverOperator* IGA::selectCrossover(std::string name, int dimension) {
	CrossoverOperator* crossover = nullptr;

	name = toLower(name);

	if (name == "order") {
		crossover = new OrderCrossover(dimension);
	} else if (name == "pmx") {
		crossover = new PMXCrossover(dimension);
	} else if (name == "cycle") {
		crossover = new CycleCrossover(dimension);
	} else {
		throw invalid_argument(name + " is not a valid crossover operator");
	}

	return crossover;
}

SelectionOperator* IGA::selectSelection(std::string name, int populationSize) {
	SelectionOperator* selector = nullptr;

	name = toLower(name);

	if (name == "fitness") {
		selector = new FitnessProportionalSelection(populationSize);
	} else if (name == "tournament") {
		selector = new TournamentSelection(tournamentSize);
	} else if (name == "elitism") {
		selector = new ElitismSelection();
	} else {
		throw invalid_argument(name + " is not a valid selection operator");
	}

	return selector;
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
	if (ga != nullptr && ga->runningAlg.load()) {
		throw runtime_error("genetic algorithm still running...");
	}

	if (contains(command, "file")) {
		cout << "loading file" << endl;
		string fileName = getLabelledValue(command, "file", loadLabels);
		fileName = "./" + fileName + ".txt";

		tspProblem->parseFile(fileName);
		cout << "loaded" << endl;
		delete ga;
		ga = nullptr;
	} else if (contains(command, "instance")) {
		cout << "loading instance" << endl;
		string instance = getLabelledValue(command, "instance", loadLabels);

		instance = trim(instance);

		vector<string> lines = splitString(instance);

		tspProblem->parseCustomInstance(lines);
		cout << "loaded" << endl;
		delete ga;
		ga = nullptr;
	}
}

void IGA::processStart(std::string command) {
	cout << 1 << endl;
	if (ga != nullptr && ga->runningAlg.load()) {
		throw runtime_error("genetic algorithm still running...");
	}
	cout << 2 << endl;
	if (gaThread.joinable()) {
		gaThread.join();
		delete ga;
	}
	cout << 3 << endl;
	if (tspProblem->dimension == 0) {
		throw invalid_argument("problem has not been loaded");
	}
	cout << 4 << endl;
	int maxGenerations = 20000;
	int populationSize = 50;

	MutationOperator* mutator = nullptr;
	CrossoverOperator* crossover = nullptr;
	SelectionOperator* selector = nullptr;

	if (contains(command, "maxgenerations")) {
		int inputMaxGens = getLabelledValueInt(command, "maxgenerations", startLabels);

		if (inputMaxGens < 1000) {
			throw out_of_range("maxgenerations must be greater than 1000");
		}

		maxGenerations = inputMaxGens;
	}
	cout << 5 << endl;
	if (contains(command, "populationsize")) {
		int inputPopSize = getLabelledValueInt(command, "populationsize", startLabels);

		if (inputPopSize <= 0 || inputPopSize >= 100000) {
			throw out_of_range("populationsize must be greater than 1 and less than 100,000");
		}

		populationSize = inputPopSize;
	}
	cout << 6 << endl;
	if (contains(command, "inverover")) {
		ga = new InverOver(*tspProblem, populationSize);
		gaThread = thread(&GeneticAlgorithm::startGA, ga, maxGenerations);
		return;
	}
	cout << 7 << endl;
	if (contains(command, "mutation")) {
		string mutationInput = getLabelledValue(command, "mutation", startLabels);
		mutator = selectMutation(mutationInput);
	} else {
		mutator = new InversionMutation();
	}
	cout << 8 << endl;
	if (contains(command, "crossover")) {
		string crossoverInput = getLabelledValue(command, "crossover", startLabels);
		crossover = selectCrossover(crossoverInput, tspProblem->dimension);
	} else {
		crossover = new PMXCrossover(tspProblem->dimension);
	}
	cout << 9 << endl;
	if (contains(command, "selection")) {
		string selectionInput = getLabelledValue(command, "selection", startLabels);
		selector = selectSelection(selectionInput, populationSize);
	} else {
		selector = new TournamentSelection(tournamentSize);
	}

	cout << "started" << endl;
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
	cout << "Input: 'load instance <a,b c,d e,f ... (optional 'EOF' to end entries)>'" << endl;

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