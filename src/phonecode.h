#pragma once
#ifndef PHONECODE_H
#define PHONECODE_H

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
	using namespace std;
	extern wregex re_mobile_phone;
	extern wregex re_telephone;
	extern wregex re_national_uniform_number;
	extern wregex re_country_code;

	wstring phone2str(const wstring& phone_string, bool mobile = true);
	wstring replace_phone(const wsmatch& match);
	wstring replace_mobile(const wsmatch& match);
	wstring process_mobile_number(const wstring& phone);
	wstring process_landline_number(const wstring& phone);
	wstring process_uniform_number(const wstring& phone);
	bool is_valid_phone_number(const std::wstring& text, const std::wsmatch& match);
#endif