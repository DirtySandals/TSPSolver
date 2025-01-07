#include "IGA.h"

#include <iostream>
#include <string>

using namespace std;

int main() {
	IGA iga;
	string command = "";

	while (true) {
		getline(cin, command);
		iga.processCommand(command);
	}

	return 0;
}