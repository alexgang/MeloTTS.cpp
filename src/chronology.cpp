#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>

// 假设 num2str 和 verbalize_digit, verbalize_cardinal 函数已经实现
std::string num2str(const std::string& num);
std::string verbalize_digit(const std::string& num);
std::string verbalize_cardinal(const std::string& num);

// 数字映射
std::unordered_map<char, std::string> DIGITS = {
    {'0', "零"}, {'1', "一"}, {'2', "二"}, {'3', "三"},
    {'4', "四"}, {'5', "五"}, {'6', "六"}, {'7', "七"},
    {'8', "八"}, {'9', "九"}
};

// 特殊时间数字转换
std::string _time_num2str(const std::string& num_string) {
    std::string result = num2str(num_string.substr(num_string.find_first_not_of('0')));
    if (num_string[0] == '0') {
        result = DIGITS['0'] + result;
    }
    return result;
}

// 时刻表达式
std::regex RE_TIME(R"(([0-1]?[0-9]|2[0-3]):([0-5][0-9])(:([0-5][0-9]))?)");

// 时间范围，如8:30-12:30
std::regex RE_TIME_RANGE(R"(([0-1]?[0-9]|2[0-3]):([0-5][0-9])(:([0-5][0-9]))?(~|-)([0-1]?[0-9]|2[0-3]):([0-5][0-9])(:([0-5][0-9]))?)");

// 替换时间
std::string replace_time(const std::smatch& match) {
    bool is_range = match.size() > 5;

    std::string hour = match.str(1);
    std::string minute = match.str(2);
    std::string second = match.str(4);

    std::string result = num2str(hour) + "点";
    if (!minute.empty() && minute != "00") {
        if (std::stoi(minute) == 30) {
            result += "半";
        }
        else {
            result += _time_num2str(minute) + "分";
        }
    }
    if (!second.empty() && second != "00") {
        result += _time_num2str(second) + "秒";
    }

    if (is_range) {
        std::string hour_2 = match.str(6);
        std::string minute_2 = match.str(7);
        std::string second_2 = match.str(9);

        result += "至" + num2str(hour_2) + "点";
        if (!minute_2.empty() && minute_2 != "00") {
            if (std::stoi(minute_2) == 30) {
                result += "半";
            }
            else {
                result += _time_num2str(minute_2) + "分";
            }
        }
        if (!second_2.empty() && second_2 != "00") {
            result += _time_num2str(second_2) + "秒";
        }
    }

    return result;
}

// 日期表达式
std::regex RE_DATE(R"((\d{4}|\d{2})年((0?[1-9]|1[0-2])月)?(((0?[1-9])|((1|2)[0-9])|30|31)([日号]))?)");

// 替换日期
std::string replace_date(const std::smatch& match) {
    std::string year = match.str(1);
    std::string month = match.str(3);
    std::string day = match.str(5);
    std::string result;
    if (!year.empty()) {
        result += verbalize_digit(year) + "年";
    }
    if (!month.empty()) {
        result += verbalize_cardinal(month) + "月";
    }
    if (!day.empty()) {
        result += verbalize_cardinal(day) + match.str(9);
    }
    return result;
}

// 用 / 或者 - 分隔的 YY/MM/DD 或者
std::regex RE_DATE2(R"((\d{4})([- /.])(0[1-9]|1[012])\2(0[1-9]|[12][0-9]|3[01]))");

// 替换日期2
std::string replace_date2(const std::smatch& match) {
    std::string year = match.str(1);
    std::string month = match.str(3);
    std::string day = match.str(4);
    std::string result;
    if (!year.empty()) {
        result += verbalize_digit(year) + "年";
    }
    if (!month.empty()) {
        result += verbalize_cardinal(month) + "月";
    }
    if (!day.empty()) {
        result += verbalize_cardinal(day) + "日";
    }
    return result;
}

// 假设 num2str 和 verbalize_digit, verbalize_cardinal 函数已经实现
std::string num2str(const std::string& num) {
    // 这里是一个简单的实现，实际实现可能更复杂
    std::unordered_map<char, std::string> num_map = {
        {'0', "零"}, {'1', "一"}, {'2', "二"}, {'3', "三"},
        {'4', "四"}, {'5', "五"}, {'6', "六"}, {'7', "七"},
        {'8', "八"}, {'9', "九"}
    };
    std::string result;
    for (char ch : num) {
        result += num_map[ch];
    }
    return result;
}

std::string verbalize_digit(const std::string& num) {
    // 这里是一个简单的实现，实际实现可能更复杂
    return num2str(num);
}

std::string verbalize_cardinal(const std::string& num) {
    // 这里是一个简单的实现，实际实现可能更复杂
    return num2str(num);
}

int main() {
    // 测试时间替换
    std::string time_text = "12:30";
    std::smatch match;
    if (std::regex_search(time_text, match, RE_TIME)) {
        std::string replaced_time = replace_time(match);
        std::cout << "Replaced time: " << replaced_time << std::endl;
    }

    // 测试时间范围替换
    std::string time_range_text = "8:30-12:30";
    if (std::regex_search(time_range_text, match, RE_TIME_RANGE)) {
        std::string replaced_time_range = replace_time(match);
        std::cout << "Replaced time range: " << replaced_time_range << std::endl;
    }

    // 测试日期替换
    std::string date_text = "2021年12月31日";
    if (std::regex_search(date_text, match, RE_DATE)) {
        std::string replaced_date = replace_date(match);
        std::cout << "Replaced date: " << replaced_date << std::endl;
    }

    // 测试日期2替换
    std::string date2_text = "2021-12-31";
    if (std::regex_search(date2_text, match, RE_DATE2)) {
        std::string replaced_date2 = replace_date2(match);
        std::cout << "Replaced date2: " << replaced_date2 << std::endl;
    }

    return 0;
}