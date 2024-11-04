#pragma once
#ifndef QUANTIFIER_H
#define QUANTIFIER_H
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
namespace text_normalization {
	// extern unordered_map<string, string> measure_dict;
	extern std::unordered_map<std::wstring, std::wstring> measure_dict;
	// extern regex re_temperature;
	extern std::wregex re_temperature;

	// string replace_temperature(const smatch& match);
	std::wstring replace_temperature(const std::wsmatch & match);
	// string replace_measure(string sentence);
	std::wstring replace_measure(std::wstring sentence);
}
#endif