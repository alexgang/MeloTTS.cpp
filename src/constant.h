#pragma once
#ifndef CONSTANT_H
#define CONSTANT_H
#include <unordered_map>


	// 初始化全角 -> 半角 映射表
	extern std::unordered_map<wchar_t, wchar_t> F2H_ASCII_LETTERS;
	extern std::unordered_map<wchar_t, wchar_t> H2F_ASCII_LETTERS;
	extern std::unordered_map<wchar_t, wchar_t> F2H_DIGITS;
	extern std::unordered_map<wchar_t, wchar_t> H2F_DIGITS;
	extern std::unordered_map<wchar_t, wchar_t> F2H_PUNCTUATIONS;
	extern std::unordered_map<wchar_t, wchar_t> H2F_PUNCTUATIONS;
	extern std::unordered_map<wchar_t, wchar_t> F2H_SPACE;
	extern std::unordered_map<wchar_t, wchar_t> H2F_SPACE;

	void initialize_constant_maps();
	std::wstring fullwidth_to_halfwidth(const std::wstring& input);
	std::wstring halfwidth_to_fullwidth(const std::wstring& input);

#endif