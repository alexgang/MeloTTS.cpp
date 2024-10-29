#include <iostream>
#include <unordered_map>
#include <string>
#include <regex>
#include <cctype>

// 定义字符映射
std::unordered_map<char32_t, char32_t> F2H_ASCII_LETTERS;
std::unordered_map<char32_t, char32_t> H2F_ASCII_LETTERS;
std::unordered_map<char32_t, char32_t> F2H_DIGITS;
std::unordered_map<char32_t, char32_t> H2F_DIGITS;
std::unordered_map<char32_t, char32_t> F2H_PUNCTUATIONS;
std::unordered_map<char32_t, char32_t> H2F_PUNCTUATIONS;
std::unordered_map<char32_t, char32_t> F2H_SPACE = { {0x3000, ' '} };
std::unordered_map<char32_t, char32_t> H2F_SPACE = { {' ', 0x3000} };

// 初始化字符映射
void initialize_maps() {
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        F2H_ASCII_LETTERS[ch + 65248] = ch;
        H2F_ASCII_LETTERS[ch] = ch + 65248;
    }
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        F2H_ASCII_LETTERS[ch + 65248] = ch;
        H2F_ASCII_LETTERS[ch] = ch + 65248;
    }
    for (char ch = '0'; ch <= '9'; ++ch) {
        F2H_DIGITS[ch + 65248] = ch;
        H2F_DIGITS[ch] = ch + 65248;
    }
    std::string punctuations = R"(!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)";
    for (char ch : punctuations) {
        F2H_PUNCTUATIONS[ch + 65248] = ch;
        H2F_PUNCTUATIONS[ch] = ch + 65248;
    }
}

// 非"有拼音的汉字"的字符串，可用于NSW提取
std::regex RE_NSW(R"((?:[^\u3007\u3400-\u4dbf\u4e00-\u9fff\uf900-\ufaff\u20000-\u2a6df\u2a703-\u2b73f\u2b740-\u2b81d\u2f80a-\u2fa1f])+)");