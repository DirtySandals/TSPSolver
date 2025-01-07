#pragma once

#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>
#include <vector>

namespace StringUtil {
	std::string trim(const std::string& str);
	std::string toLower(const std::string& str);
	std::vector<std::string> split(std::string& str, char c);
	std::vector<std::string> splitString(std::string& str);
	bool contains(const std::string& str, char ch);
	bool contains(const std::string& str1, const std::string& str2);
	int indexOf(const std::string& str1, const std::string& str2);
	bool isDigitString(const std::string& str);
	std::string commaSeparator(int integer);
};

#endif // !STRINGUTIL_H