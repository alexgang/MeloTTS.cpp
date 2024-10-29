#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;

unordered_map<string, string> DIGITS = {
    {"0", "零"}, {"1", "一"}, {"2", "二"}, {"3", "三"}, {"4", "四"},
    {"5", "五"}, {"6", "六"}, {"7", "七"}, {"8", "八"}, {"9", "九"}
};

string verbalize_digit(const string& value_string, bool alt_one = false) {
    string result;
    for (const char& digit : value_string) {
        result += DIGITS[string(1, digit)];
    }
    if (alt_one) {
        size_t pos = 0;
        while ((pos = result.find("一", pos)) != string::npos) {
            result.replace(pos, 1, "幺");
            pos += 1;
        }
    }
    return result;
}

string phone2str(const string& phone_string, bool mobile = true) {
    string result;
    if (mobile) {
        stringstream ss(phone_string);
        string part;
        vector<string> parts;
        while (getline(ss, part, ' ')) {
            parts.push_back(verbalize_digit(part, true));
        }
        for (size_t i = 0; i < parts.size(); ++i) {
            result += parts[i];
            if (i != parts.size() - 1) {
                result += "，";
            }
        }
    }
    else {
        stringstream ss(phone_string);
        string part;
        vector<string> parts;
        while (getline(ss, part, '-')) {
            parts.push_back(verbalize_digit(part, true));
        }
        for (size_t i = 0; i < parts.size(); ++i) {
            result += parts[i];
            if (i != parts.size() - 1) {
                result += "，";
            }
        }
    }
    return result;
}

string replace_phone(const smatch& match) {
    return phone2str(match.str(0), false);
}

string replace_mobile(const smatch& match) {
    return phone2str(match.str(0));
}

string process_mobile_number(const string& phone) {
    // 匹配并处理国家代码部分
    regex re_country_code(R"(\+?86 ?)");
    string result = regex_replace(phone, re_country_code, "中国，");

    // 剩下的手机号部分
    regex re_mobile_body(R"(\d{11})");
    smatch match;
    if (regex_search(result, match, re_mobile_body)) {
        string mobile_number = match.str(0);
        result = regex_replace(result, re_mobile_body, phone2str(mobile_number, true)); // 使用 verbalize_digit 处理数字
    }

    return result;
}

string process_landline_number(const string& phone) {
    // 匹配区号部分
    regex re_area_code(R"(0\d{2,3})");
    smatch match;
    string result = phone;

    if (regex_search(phone, match, re_area_code)) {
        string area_code = match.str(0);
        result = regex_replace(result, re_area_code, verbalize_digit(area_code) + "，");
    }

    // 匹配剩余的电话号码部分
    regex re_phone_body(R"(\d{6,8})");
    if (regex_search(result, match, re_phone_body)) {
        string phone_body = match.str(0);
        result = regex_replace(result, re_phone_body, verbalize_digit(phone_body, true));
    }

    return result;
}

string process_uniform_number(const string& phone) {
    // 匹配400号码
    regex re_400(R"(400)");
    smatch match;
    string result = phone;

    if (regex_search(phone, match, re_400)) {
        result = regex_replace(result, re_400, "四，零，零");
    }

    // 匹配剩余的号码部分
    regex re_phone_body(R"(\d{3}-\d{4})");
    if (regex_search(result, match, re_phone_body)) {
        string phone_body = match.str(0);
        result = regex_replace(result, re_phone_body, verbalize_digit(phone_body, true));
    }

    return result;
}

int main() {
    // 手机号码示例
    string mobile_example = "+8613912345678";
    cout << "处理后的手机号: " << process_mobile_number(mobile_example) << endl;

    // 固定电话号码示例
    string telephone_example = "010-12345678";
    cout << "处理后的固定电话: " << process_landline_number(telephone_example) << endl;

    // 全国统一号码示例
    string national_uniform_number_example = "400-123-4567";
    cout << "处理后的400号码: " << process_uniform_number(national_uniform_number_example) << endl;

    return 0;
}