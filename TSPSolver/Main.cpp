#include "IGA.h"

#include <iostream>
#include <string>

using namespace std;

int main() {
	// Create interface
	IGA iga;
	string command = "";

	// Keep getting commands
	while (true) {
		// Get command
		getline(cin, command);
		// Process command
		iga.processCommand(command);
	}

	return 0;
}