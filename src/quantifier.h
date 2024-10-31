#pragma once
#ifndef QUANTIFIER_H
#define QUANTIFIER_H
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
	using namespace std;

	// extern unordered_map<string, string> measure_dict;
	extern unordered_map<wstring, wstring> measure_dict;
	// extern regex re_temperature;
	extern wregex re_temperature;

	// string replace_temperature(const smatch& match);
	wstring replace_temperature(const wsmatch& match);
	// string replace_measure(string sentence);
	wstring replace_measure(wstring sentence);

#endif