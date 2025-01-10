#include "IGA.h"

#include "InverOver.h"
#include "StringUtil.h"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;
using namespace StringUtil;

// Initialise static const vectors and strings
const vector<string> IGA::fileNames = { "a280", "eil51", "eil76", "eil101", "kroA100", "kroC100", "lin105", "pcb442", "pr2392", "st70", "usa13509" };
const vector<string> IGA::loadLabels = { "load", "file", "instance" };
const vector<string> IGA::startLabels = { "start", "maxgenerations", "populationsize", "mutation", "crossover", "selection" };
const string IGA::author = "Aleksander Gayko";
const string IGA::programName = "TSPSolver";

// Default constructor
IGA::IGA() {
	tspProblem = new TSPProblem();
	// Start print statement
	cout << programName << " by " << author << endl;
	cout << "type: 'help' for help" << endl;
}

MutationOperator* IGA::selectMutation(std::string name) {
	MutationOperator* mutator = nullptr;

	name = toLower(name);
	// Creates mutation operator associated name
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
	// Creates crossover operator associated name
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
	// Creates selection operator associated name
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

// Delete genetic algorithm and problem
IGA::~IGA() {
	delete ga;
	delete tspProblem;
}

void IGA::processCommand(string command) {
	try {
		// Delete leading and ending space
		command = trim(command);
		// First word determines commandType
		const string commandType = toLower(splitString(command)[0]);
		// Determine which command it is
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
	} 
	// Catch any errors to prevent malicious inputs
	catch (exception& e) {
		cerr << e.what() << endl;
	}
}

void IGA::processLoad(std::string command) {
	// If algorithm still running, do not load
	if (ga != nullptr && ga->runningAlg.load()) {
		throw runtime_error("genetic algorithm still running...");
	}
	// Load instance from file
	if (contains(command, "file")) {
		string fileName = getLabelledValue(command, "file", loadLabels);
		fileName = "./" + fileName + ".txt";
		// Create problem
		tspProblem->parseFile(fileName);
		// Delete genetic algorithm in case
		delete ga;
		ga = nullptr;
	} 
	// Load instance from terminal
	else if (contains(command, "instance")) {
		string instance = getLabelledValue(command, "instance", loadLabels);

		instance = trim(instance);

		vector<string> lines = splitString(instance);
		// Create problem
		tspProblem->parseCustomInstance(lines);
		// Delete genetic algorithm in case
		delete ga;
		ga = nullptr;
	}
}

void IGA::processStart(std::string command) {
	// If algorithm still running, do not start
	if (ga != nullptr && ga->runningAlg.load()) {
		throw runtime_error("genetic algorithm still running...");
	}
	// If not running, join handing thread
	if (gaThread.joinable()) {
		gaThread.join();
		delete ga;
	}
	// If problem not loaded, do not start
	if (tspProblem->dimension == 0) {
		throw invalid_argument("problem has not been loaded");
	}
	// Default parameters
	int maxGenerations = std::numeric_limits<int>::max();
	int populationSize = 50;

	MutationOperator* mutator = nullptr;
	CrossoverOperator* crossover = nullptr;
	SelectionOperator* selector = nullptr;

	// If maxgenerations specified, use that value
	if (contains(command, "maxgenerations")) {
		int inputMaxGens = getLabelledValueInt(command, "maxgenerations", startLabels);

		if (inputMaxGens < 1000) {
			throw out_of_range("maxgenerations must be greater than 1000");
		}

		maxGenerations = inputMaxGens;
	}
	// If populationsize specified, use that value
	if (contains(command, "populationsize")) {
		int inputPopSize = getLabelledValueInt(command, "populationsize", startLabels);

		if (inputPopSize <= 0 || inputPopSize >= 100000) {
			throw out_of_range("populationsize must be greater than 1 and less than 100,000");
		}

		populationSize = inputPopSize;
	}
	// If inverover specified, use the inverover derived class algorithm
	if (contains(command, "inverover")) {
		ga = new InverOver(*tspProblem, populationSize);
		gaThread = thread(&GeneticAlgorithm::startGA, ga, maxGenerations);
		return;
	}
	// If mutation specified, use it
	if (contains(command, "mutation")) {
		string mutationInput = getLabelledValue(command, "mutation", startLabels);
		mutator = selectMutation(mutationInput);
	}
	// Else, use inversion mutation as default
	else {
		mutator = new InversionMutation();
	}
	// If crossover specified, use it
	if (contains(command, "crossover")) {
		string crossoverInput = getLabelledValue(command, "crossover", startLabels);
		crossover = selectCrossover(crossoverInput, tspProblem->dimension);
	} 
	// Else, use PMX crossover as default
	else {
		crossover = new PMXCrossover(tspProblem->dimension);
	}
	// If selection specified, use it
	if (contains(command, "selection")) {
		string selectionInput = getLabelledValue(command, "selection", startLabels);
		selector = selectSelection(selectionInput, populationSize);
	} 
	// Else use tournament selection as default
	else {
		selector = new TournamentSelection(tournamentSize);
	}

	// Start algorithm
	ga = new GeneticAlgorithm(*tspProblem, populationSize, mutator, crossover, selector);
	gaThread = thread(&GeneticAlgorithm::startGA, ga, maxGenerations);
}

void IGA::stop() {
	if (ga == nullptr) {
		return;
	}
	// If no hanging thread, don't bother
	if (!gaThread.joinable()) {
		return;
	}
	// Stop algorithm (if it even is running)
	ga->stop();
	// Give algorithm time to cease
	this_thread::sleep_for(chrono::milliseconds(100));
	// Join hanging thread
	gaThread.join();
	// Give thread time to join
	this_thread::sleep_for(chrono::milliseconds(100));
}

void IGA::quit() {
	// Stop algorithm in case
	stop();
	// Exit program
	exit(0);
}

void IGA::help() {
	// Help page for interface commands
	cout << "HELP" << endl;
	cout << endl;
	cout << "LOAD" << endl;
	cout << "To use the program, you must first load an instance via file name or a custom instance input." << endl;
	cout << "The instance must also have more than three entries to be considered" << endl;
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
	cout << "Input: 'start maxgenerations <value : default int_max> populationsize <value : default 50> mutation <value : default 'inversion'> crossover <value : default 'pmx'> selection <value : default 'tournament'>" << endl;
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
	cout << "Input: 'start inverover maxgenerations <value : default int_max> populationsize <value : default 50>'" << endl;

	cout << "STOP" << endl;
	cout << "To stop the current genetic algorithm, type: 'stop'." << endl;
	
	cout << endl;
}

int IGA::getLabelledValueInt(string text, string label, const vector<string> allLabels) {
	// Get string value
	string valueString = getLabelledValue(text, label, allLabels);
	string resultString = splitString(valueString)[0];
	// Convert to int
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
	// Start and possible end of value
	int valueStart = indexOf(text, label) + label.size();
	int valueEnd = text.size();

	// Search for strings between label and next label in alllabels
	for (int i = 0; i < allLabels.size(); i++) {
		if (allLabels[i] != label && contains(text, allLabels[i])) {
			int otherIDStartIndex = indexOf(text, allLabels[i]);
			if (otherIDStartIndex > valueStart && otherIDStartIndex < valueEnd) {
				valueEnd = otherIDStartIndex;
			}
		}
	}
	// Return trimmed string between labels to get value
	return trim(text.substr(valueStart, valueEnd - valueStart));
}