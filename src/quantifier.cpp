#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

using namespace std;

unordered_map<string, string> DIGITS = {
    {"0", "零"}, {"1", "一"}, {"2", "二"}, {"3", "三"}, {"4", "四"},
    {"5", "五"}, {"6", "六"}, {"7", "七"}, {"8", "八"}, {"9", "九"}
};

unordered_map<string, string> measure_dict = {
    {"cm2", "平方厘米"}, {"cm²", "平方厘米"}, {"cm3", "立方厘米"}, {"cm³", "立方厘米"},
    {"cm", "厘米"}, {"db", "分贝"}, {"ds", "毫秒"}, {"kg", "千克"}, {"km", "千米"},
    {"m2", "平方米"}, {"m²", "平方米"}, {"m³", "立方米"}, {"m3", "立方米"},
    {"ml", "毫升"}, {"m", "米"}, {"mm", "毫米"}, {"s", "秒"}
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

string num2str(const string& value_string) {
    vector<string> integer_decimal;
    size_t pos = value_string.find('.');
    if (pos == string::npos) {
        integer_decimal.push_back(value_string);
    }
    else {
        integer_decimal.push_back(value_string.substr(0, pos));
        integer_decimal.push_back(value_string.substr(pos + 1));
    }

    string integer = integer_decimal[0];
    string decimal = (integer_decimal.size() == 2) ? integer_decimal[1] : "";

    string result = verbalize_digit(integer);
    decimal.erase(decimal.find_last_not_of('0') + 1);
    if (!decimal.empty()) {
        result = result.empty() ? "零" : result;
        result += "点" + verbalize_digit(decimal);
    }
    return result;
}

string replace_temperature(const smatch& match) {
    string sign = match.str(1);
    string temperature = match.str(2);
    string unit = match.str(4);
    sign = sign.empty() ? "" : "零下";
    temperature = num2str(temperature);
    unit = (unit == "摄氏度") ? "摄氏度" : "度";
    return sign + temperature + unit;
}

string replace_measure(string sentence) {
    for (const auto& q_notation : measure_dict) {
        size_t pos = 0;
        while ((pos = sentence.find(q_notation.first, pos)) != string::npos) {
            sentence.replace(pos, q_notation.first.length(), q_notation.second);
            pos += q_notation.second.length();
        }
    }
    return sentence;
}

int main() {
    // Example usage
    string temperature_example = "-3°C";
    regex re_temperature(R"((-?)(\d+(\.\d+)?)(°C|℃|度|摄氏度))");
    smatch match;
    if (regex_search(temperature_example, match, re_temperature)) {
        cout << replace_temperature(match) << endl;
    }

    string measure_example = "10cm2";
    cout << replace_measure(measure_example) << endl;

    return 0;
}