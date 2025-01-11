#pragma once

#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
/*
 * StringUtil namespace is a collection of function from my SandalBotV2 repo
 * because C++ never has the function you actually want
 */
namespace StringUtil {
	// Trims the prefix and suffix of empty spaces
	inline std::string trim(const std::string& str) {
		size_t start = str.find_first_not_of(" \t\n\r\f\v");
		size_t end = str.find_last_not_of(" \t\n\r\f\v");

		return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
	}
	// Converts string to lowercase
	inline std::string toLower(const std::string& str) {
		std::string lowerStr = str;
		// Use lambda function to convert each char to lowercase using standard tolower function
		std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) { return std::tolower(c); });
		return lowerStr;
	}
	// Splits string into vector by splitting via char parameter c
	inline std::vector<std::string> split(std::string& str, char c) {
		// Parse str and add to vector each time c is encountered
		vector<string> lines;
		string line = "";

		for (int i = 0; i < str.size(); i++) {
			if (str[i] == c) {
				lines.push_back(line);
				line = "";
			} else {
				line += str[i];
			}
		}
		// If remaining line, push to vector
		if (line.size() > 0) {
			lines.push_back(line);
		}

		return lines;
	}
	// Splits string into vector by splitting via spaces between characters
	inline vector<string> splitString(string& str) {
		// Parse str and add each word between spaces to vector
		vector<string> words;
		stringstream ss(str);
		string word;

		while (ss >> word) {
			words.push_back(word);
		}
		if (!words.size()) {
			words.push_back("");
		}

		return words;
	}
	// Returns true if string contains character ch
	inline bool contains(const std::string& str, char ch) {
		return str.find(ch) != std::string::npos;
	}
	// Returns true if str1 contains str2
	inline bool contains(const std::string& str1, const std::string& str2) {
		return str1.find(str2) != std::string::npos;
	}
	// Returns beginning index of str2 in str1
	inline int indexOf(const std::string& str1, const std::string& str2) {
		int index = str1.find(str2);
		if (index == std::string::npos) {
			return -1;
		}
		return index;
	}
	// Returns true if every character in string is a digit
	inline bool isDigitString(const std::string& str) {
		if (!str.size()) {
			return false;
		}

		for (char ch : str) {
			if (!std::isdigit(static_cast<unsigned char>(ch))) {
				return false;
			}
		}
		return true;
	}
// Returns string that converts integer into string splitting the significant digits by commas
	inline string commaSeparator(int integer) {
		string stringInt = to_string(integer);
		size_t size = stringInt.size();
		for (int i = 0; i < size; i++) {
			if (i != 0 && i != size - 1 && (i + 1) % 3 == 0 && stringInt[size - 1 - i] != '-') {
				stringInt.insert(size - 1 - i, 1, ',');
			}
		}
		return stringInt;
	}
};

#endif // !STRINGUTIL_H