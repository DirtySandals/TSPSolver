#pragma once

#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>
#include <vector>

/*
 * StringUtil namespace is a collection of function from my SandalBotV2 repo
 * because C++ never has the function you actually want
 */
namespace StringUtil {
	// Trims the prefix and suffix of empty spaces
	std::string trim(const std::string& str);
	// Converts string to lowercase
	std::string toLower(const std::string& str);
	// Splits string into vector by splitting via char parameter c
	std::vector<std::string> split(std::string& str, char c);
	// Splits string into vector by splitting via spaces between characters
	std::vector<std::string> splitString(std::string& str);
	// Returns true if string contains character ch
	bool contains(const std::string& str, char ch);
	// Returns true if str1 contains str2
	bool contains(const std::string& str1, const std::string& str2);
	// Returns beginning index of str2 in str1
	int indexOf(const std::string& str1, const std::string& str2);
	// Returns true if every character in string is a digit
	bool isDigitString(const std::string& str);
	// Returns string that converts integer into string splitting the significant digits by commas
	std::string commaSeparator(int integer);
};

#endif // !STRINGUTIL_H