#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cwctype>  // 包含 iswdigit 所需的头文件
#include"number.h"

#ifdef _WIN32
#include <iostream>
#include <locale>
#define NOMINMAX
#define NOGDI
#define NOCRYPT
#include <windows.h>
#endif
using namespace std;
//wregex re_mobile_phone(LR"((?<!\d)((\+?86 ?)?1([38]\d|5[0-35-9]|7[678]|9[89])\d{8})(?!\d))");
//wregex re_telephone(LR"((?<!\d)((0(10|2[1-3]|[3-9]\d{2})-?)?[1-9]\d{6,7})(?!\d))");
//wregex re_national_uniform_number(LR"((400)(-)?\d{3}(-)?\d{4}))");
std::wregex re_mobile_phone(LR"((\+?86 ?)?1([38]\d|5[0-35-9]|7[678]|9[89])\d{8})");
std::wregex re_telephone(LR"((0(10|2[1-3]|[3-9]\d{2})-?)?[1-9]\d{6,7})");
std::wregex re_national_uniform_number(LR"(400-?\d{3}-?\d{4})");


// 手动检查是否有前后数字
bool is_valid_phone_number(const std::wstring& text, const std::wsmatch& match) {
    // 检查手机号前面和后面的字符是否为数字
    if (match.position() > 0 && std::iswdigit(text[match.position() - 1])) {
        return false;  // 前面有数字，不符合要求
    }
    if (match.position() + match.length() < text.size() && std::iswdigit(text[match.position() + match.length()])) {
        return false;  // 后面有数字，不符合要求
    }
    return true;
}

wstring phone2str(const wstring& phone_string, bool mobile = true) {
    wstring result;
    if (mobile) {
        wstringstream ss(phone_string);
        wstring part;
        vector<wstring> parts;
        while (getline(ss, part, L' ')) {
            parts.push_back(verbalize_digit(part, true));
        }
        for (size_t i = 0; i < parts.size(); ++i) {
            result += parts[i];
            if (i != parts.size() - 1) {
                result += L"，";
            }
        }
    }
    else {
        wstringstream ss(phone_string);
        wstring part;
        vector<wstring> parts;
        while (getline(ss, part, L'-')) {
            parts.push_back(verbalize_digit(part, true));
        }
        for (size_t i = 0; i < parts.size(); ++i) {
            result += parts[i];
            if (i != parts.size() - 1) {
                result += L"，";
            }
        }
    }
    return result;
}

wstring replace_phone(const wsmatch& match) {
    return phone2str(match.str(0), false);
}

wstring replace_mobile(const wsmatch& match) {
    return phone2str(match.str(0));
}

wstring process_mobile_number(const wstring& phone) {
    // 匹配并处理国家代码部分
    wregex re_country_code(LR"(\+?86 ?)");
    wstring result = regex_replace(phone, re_country_code, L"中国，");

    // 剩下的手机号部分
    wregex re_mobile_body(LR"(\d{11})");
    wsmatch match;
    if (regex_search(result, match, re_mobile_body)) {
        wstring mobile_number = match.str(0);
        result = regex_replace(result, re_mobile_body, phone2str(mobile_number, true)); // 使用 verbalize_digit 处理数字
    }

    return result;
}

wstring process_landline_number(const wstring& phone) {
    // 匹配区号部分
    wregex re_area_code(LR"(0\d{2,3})");
    wsmatch match;
    wstring result = phone;

    if (regex_search(phone, match, re_area_code)) {
        wstring area_code = match.str(0);
        result = regex_replace(result, re_area_code, verbalize_digit(area_code) + L"，");
    }

    // 匹配剩余的电话号码部分
    wregex re_phone_body(LR"(\d{6,8})");
    if (regex_search(result, match, re_phone_body)) {
        wstring phone_body = match.str(0);
        result = regex_replace(result, re_phone_body, verbalize_digit(phone_body, true));
    }

    return result;
}

wstring process_uniform_number(const wstring& phone) {
    // 匹配400号码
    wregex re_400(LR"(400)");
    wsmatch match;
    wstring result = phone;

    if (regex_search(phone, match, re_400)) {
        result = regex_replace(result, re_400, L"四，零，零");
    }

    // 匹配剩余的号码部分
    wregex re_phone_body(LR"(\d{3}-\d{4})");
    if (regex_search(result, match, re_phone_body)) {
        wstring phone_body = match.str(0);
        result = regex_replace(result, re_phone_body, verbalize_digit(phone_body, true));
    }

    return result;
}

//int main() {
//    #ifdef _WIN32
//        SetConsoleOutputCP(CP_UTF8);
//        // 使用系统默认区域设置
//        std::wcout.imbue(std::locale(""));
//    #endif
//    // 手机号码示例
//    wstring mobile_example = L"+8613912345678";
//    wcout << L"处理后的手机号: " << process_mobile_number(mobile_example) << endl;
//
//    // 固定电话号码示例
//    wstring telephone_example = L"010-12345678";
//    wcout << L"处理后的固定电话: " << process_landline_number(telephone_example) << endl;
//
//    // 全国统一号码示例
//    wstring national_uniform_number_example = L"400-123-4567";
//    wcout << L"处理后的400号码: " << process_uniform_number(national_uniform_number_example) << endl;
//
//    return 0;
//}