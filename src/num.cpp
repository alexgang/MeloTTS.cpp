#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <regex>
#include <vector>
#include <sstream>
#include <functional>

using namespace std;

unordered_map<char, string> DIGITS = {
    {'0', "零"}, {'1', "一"}, {'2', "二"}, {'3', "三"}, {'4', "四"},
    {'5', "五"}, {'6', "六"}, {'7', "七"}, {'8', "八"}, {'9', "九"}
};

map<int, string> UNITS = {
    {1, "十"}, {2, "百"}, {3, "千"}, {4, "万"}, {8, "亿"}
};

string num2str(const string& value_string);

string replace_frac(const smatch& match) {
    string sign = match.str(1);
    string nominator = match.str(2);
    string denominator = match.str(3);
    sign = sign.empty() ? "" : "负";
    nominator = num2str(nominator);
    denominator = num2str(denominator);
    return sign + denominator + "分之" + nominator;
}

string replace_percentage(const smatch& match) {
    string sign = match.str(1);
    string percent = match.str(2);
    sign = sign.empty() ? "" : "负";
    percent = num2str(percent);
    return sign + "百分之" + percent;
}

string replace_negative_num(const smatch& match) {
    string sign = match.str(1);
    string number = match.str(2);
    sign = sign.empty() ? "" : "负";
    number = num2str(number);
    return sign + number;
}

string replace_default_num(const smatch& match) {
    string number = match.str(0);
    string result;
    for (char digit : number) {
        result += DIGITS[digit];
    }
    return result;
}

unordered_map<char, string> asmd_map = {
    {'+', "加"}, {'-', "减"}, {'×', "乘"}, {'÷', "除"}, {'=', "等于"}
};

string replace_asmd(const smatch& match) {
    string result = match.str(1) + asmd_map[match.str(8)[0]] + match.str(9);
    return result;
}

string replace_positive_quantifier(const smatch& match) {
    string number = match.str(1);
    string match_2 = match.str(2);
    if (match_2 == "+") {
        match_2 = "多";
    }
    string quantifiers = match.str(3);
    number = num2str(number);
    return number + match_2 + quantifiers;
}

string replace_number(const smatch& match) {
    string sign = match.str(1);
    string number = match.str(2);
    string pure_decimal = match.str(5);
    if (!pure_decimal.empty()) {
        return num2str(pure_decimal);
    }
    else {
        sign = sign.empty() ? "" : "负";
        number = num2str(number);
        return sign + number;
    }
}

string replace_with_callback(const string& input, const regex& re, const function<string(const smatch&)>& callback) {
    string output;
    auto begin = sregex_iterator(input.begin(), input.end(), re);
    auto end = sregex_iterator();

    size_t last_pos = 0;
    for (auto it = begin; it != end; ++it) {
        smatch match = *it;
        output += input.substr(last_pos, match.position() - last_pos); // 复制未匹配部分
        output += callback(match); // 使用回调函数替换匹配项
        last_pos = match.position() + match.length(); // 移动最后的位置
    }
    output += input.substr(last_pos); // 复制剩余的未匹配部分
    return output;
}

string replace_range(const smatch& match) {
    string first = match.str(1);
    string second = match.str(6);

    regex re("(-?)((\\d+)(\\.\\d+)?)|(\\.(\\d+))");

    // 使用回调替换 first 和 second
    first = replace_with_callback(first, re, replace_number);
    second = replace_with_callback(second, re, replace_number);

    return first + "到" + second;
}

string replace_to_range(const smatch& match) {
    string result = match.str(0);
    result.replace(result.find('~'), 1, "至");
    return result;
}

vector<string> _get_value(const string& value_string, bool use_zero = true) {
    string stripped = value_string;
    stripped.erase(0, min(stripped.find_first_not_of('0'), stripped.size() - 1));
    if (stripped.empty()) {
        return {};
    }
    else if (stripped.size() == 1) {
        if (use_zero && stripped.size() < value_string.size()) {
            return { DIGITS['0'], DIGITS[stripped[0]] };
        }
        else {
            return { DIGITS[stripped[0]] };
        }
    }
    else {
        int largest_unit = 0;
        for (auto it = UNITS.rbegin(); it != UNITS
            .rend(); ++it) {
            if (it->first < stripped.size()) {
                largest_unit = it->first;
                break;
            }
        }
        string first_part = value_string.substr(0, value_string.size() - largest_unit);
        string second_part = value_string.substr(value_string.size() - largest_unit);
        vector<string> result = _get_value(first_part);
        result.push_back(UNITS[largest_unit]);
        vector<string> second_result = _get_value(second_part);
        result.insert(result.end(), second_result.begin(), second_result.end());
        return result;
    }
}

string verbalize_cardinal(const string& value_string) {
    if (value_string.empty()) {
        return "";
    }

    string stripped = value_string;
    stripped.erase(0, min(stripped.find_first_not_of('0'), stripped.size() - 1));
    if (stripped.empty()) {
        return DIGITS['0'];
    }

    vector<string> result_symbols = _get_value(value_string);
    if (result_symbols.size() >= 2 && result_symbols[0] == DIGITS['1'] && result_symbols[1] == UNITS[1]) {
        result_symbols.erase(result_symbols.begin());
    }
    string result;
    for (const string& symbol : result_symbols) {
        result += symbol;
    }
    return result;
}

string verbalize_digit(const string& value_string, bool alt_one = false) {
    string result;
    for (char digit : value_string) {
        result += DIGITS[digit];
    }
    if (alt_one) {
        size_t pos = result.find("一");
        while (pos != string::npos) {
            result.replace(pos, 1, "幺");
            pos = result.find("一", pos + 1);
        }
    }
    return result;
}

string num2str(const string& value_string) {
    size_t point_pos = value_string.find('.');
    string integer, decimal;
    if (point_pos == string::npos) {
        integer = value_string;
    }
    else {
        integer = value_string.substr(0, point_pos);
        decimal = value_string.substr(point_pos + 1);
    }

    string result = verbalize_cardinal(integer);

    decimal.erase(decimal.find_last_not_of('0') + 1);
    if (!decimal.empty()) {
        result = result.empty() ? "零" : result;
        result += "点" + verbalize_digit(decimal);
    }
    return result;
}

int main() {
    // 测试示例
    string test_frac = "3/4";
    string test_percentage = "50%";
    string test_negative_num = "-10";
    string test_default_num = "00078";
    string test_asmd = "3+2";
    string test_positive_quantifier = "3个";
    string test_number = "3.14";
    string test_range = "1-10";
    string test_to_range = "1~10%";

    regex re_frac(R"((-?)(\d+)/(\d+))");
    regex re_percentage(R"((-?)(\d+(\.\d+)?)%)");
    regex re_negative_num(R"((-)(\d+))");
    regex re_default_num(R"(\d{3}\d*)");
    regex re_asmd(R"(((-?)((\d+)(\.\d+)?)|(\.(\d+)))([\+\-\×÷=])((-?)((\d+)(\.\d+)?)|(\.(\d+))))");
    regex re_positive_quantifier(R"((\d+)([多余几\+])?(封|艘|把|目|套|段|人|所|朵|匹|张|座|回|场|尾|条|个|首|阙|阵|网|炮|顶|丘|棵|只|支|袭|辆|挑|担|颗|壳|窠|曲|墙|群|腔|砣|座|客|贯|扎|捆|刀|令|打|手|罗|坡|山|岭|江|溪|钟|队|单|双|对|出|口|头|脚|板|跳|枝|件|贴|针|线|管|名|位|身|堂|课|本|页|家|户|层|丝|毫|厘|分|钱|两|斤|担|铢|石|钧|锱|忽|(千|毫|微)克|毫|厘|(公)分|分|寸|尺|丈|里|寻|常|铺|程|(千|分|厘|毫|微)米|米|撮|勺|合|升|斗|石|盘|碗|碟|叠|桶|笼|盆|盒|杯|钟|斛|锅|簋|篮|盘|桶|罐|瓶|壶|卮|盏|箩|箱|煲|啖|袋|钵|年|月|日|季|刻|时|周|天|秒|分|小时|旬|纪|岁|世|更|夜|春|夏|秋|冬|代|伏|辈|丸|泡|粒|颗|幢|堆|条|根|支|道|面|片|张|颗|块|元|(亿|千万|百万|万|千|百)|(亿|千万|百万|万|千|百|美|)元|(亿|千万|百万|万|千|百|十|)吨|(亿|千万|百万|万|千|百|)块|角|毛|分))");
    regex re_number(R"((-?)((\d+)(\.\d+)?)|(\.(\d+)))");
    //regex re_range(R"((?<![\d\+\-\×÷=])((-?)((\d+)(\.\d+)?))[-~]((-?)((\d+)(\.\d+)?))(?![\d\+\-\×÷=]))");
    regex re_range(R"((-?\d+(\.\d+)?)[-~](-?\d+(\.\d+)?))");
    regex re_to_range(R"(((-?)((\d+)(\.\d+)?)|(\.(\d+)))(%|°C|℃|度|摄氏度|cm2|cm²|cm3|cm³|cm|db|ds|kg|km|m2|m²|m³|m3|ml|m|mm|s)[~]((-?)((\d+)(\.\d+)?)|(\.(\d+)))(%|°C|℃|度|摄氏度|cm2|cm²|cm3|cm³|cm|db|ds|kg|km|m2|m²|m³|m3|ml|m|mm|s))");

    smatch match;

    if (regex_search(test_frac, match, re_frac)) {
        cout << test_frac << " Fraction: " << replace_frac(match) << endl;
    }

    if (regex_search(test_percentage, match, re_percentage)) {
        cout << test_percentage << " Percentage: " << replace_percentage(match) << endl;
    }

    if (regex_search(test_negative_num, match, re_negative_num)) {
        cout << test_negative_num << " Negative Number: " << replace_negative_num(match) << endl;
    }

    if (regex_search(test_default_num, match, re_default_num)) {
        cout << test_default_num << " Default Number: " << replace_default_num(match) << endl;
    }

    if (regex_search(test_asmd, match, re_asmd)) {
        cout << test_asmd << " ASMD: " << replace_asmd(match) << endl;
    }

    if (regex_search(test_positive_quantifier, match, re_positive_quantifier)) {
        cout << test_positive_quantifier << " Positive Quantifier: " << replace_positive_quantifier(match) << endl;
    }

    if (regex_search(test_number, match, re_number)) {
        cout << test_number << " Number: " << replace_number(match) << endl;
    }

    if (regex_search(test_range, match, re_range)) {
        size_t pos = match.position(0);
        if ((pos == 0 || !isdigit(test_range[pos - 1])) &&
            (pos + match.length(0) == test_range.length() || !isdigit(test_range[pos + match.length(0)]))) {
                cout << test_range << " Range: " << replace_range(match) << endl;
        }
        else {
            cout << "前后有非法字符，匹配无效" << endl;
        }
    }

    if (regex_search(test_to_range, match, re_to_range)) {
        cout << test_to_range << " To Range: " << replace_to_range(match) << endl;
    }

    return 0;
}