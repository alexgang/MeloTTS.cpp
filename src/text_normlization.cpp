#include <regex>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include"Text_normlization.h"
#include"char_convert.h"
#include"constant.h"
#include"chronology.h"
#include"number.h"
#include"phonecode.h"
#include"quantifier.h"

#ifdef _WIN32
#include <iostream>
#include <locale>
// 避免 <windows.h> 定义 byte
#define NOMINMAX
#define NOGDI
#define NOCRYPT
#include <windows.h>
#endif



// 构造函数
TextNormalizer::TextNormalizer() : SENTENCE_SPLITOR(L"([：、；。？！;?!][”’]?)") {
    initialize_constant_maps();
    initialize_char_maps();
}

// 分割函数
std::vector<std::wstring> TextNormalizer::split(const std::wstring& text, const std::wstring& lang) {
    std::wstring modified_text = text;
    if (lang == L"zh") {
        //modified_text.erase(std::remove(modified_text.begin(), modified_text.end(), L' '), modified_text.end());

        modified_text = std::regex_replace(modified_text, std::wregex(L"([——《》【】<>{}()（）#&@“”^_|\\\\])"), L"");
    }
    modified_text = std::regex_replace(modified_text, SENTENCE_SPLITOR, L"$1\n");
    //modified_text.erase(std::remove(modified_text.begin(), modified_text.end(), L'\n'), modified_text.end());

    std::vector<std::wstring> sentences;
    std::wregex re(L"(\\n+)");
    std::wsregex_token_iterator it(modified_text.begin(), modified_text.end(), re, -1);
    std::wsregex_token_iterator end;

    while (it != end) {
        sentences.push_back(*it);
        ++it;
    }
    return sentences;
}

// 后处理替换函数
std::wstring TextNormalizer::post_replace(const std::wstring& sentence) {
    std::wstring modified_sentence = sentence;
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"/"), L"每");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"①"), L"一");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"②"), L"二");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"③"), L"三");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"④"), L"四");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"⑤"), L"五");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"⑥"), L"六");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"⑦"), L"七");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"⑧"), L"八");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"⑨"), L"九");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"⑩"), L"十");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"α"), L"阿尔法");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"β"), L"贝塔");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"γ|Γ"), L"伽玛");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"δ|Δ"), L"德尔塔");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"ε"), L"艾普西龙");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"ζ"), L"捷塔");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"η"), L"依塔");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"θ|Θ"), L"西塔");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"ι"), L"艾欧塔");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"κ"), L"喀帕");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"λ|Λ"), L"拉姆达");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"μ"), L"缪");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"ν"), L"拗");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"ξ|Ξ"), L"克西");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"ο"), L"欧米克伦");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"π|Π"), L"派");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"ρ"), L"肉");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"ς|σ|Σ"), L"西格玛");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"τ"), L"套");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"υ"), L"宇普西龙");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"φ|Φ"), L"服艾");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"χ"), L"器");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"ψ|Ψ"), L"普赛");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"ω|Ω"), L"欧米伽");
    modified_sentence = std::regex_replace(modified_sentence, std::wregex(L"([-——《》【】<=>{}()（）#&@“”^_|\\\\])"), L"");
    return modified_sentence;
}


std::wstring TextNormalizer::normalize_sentence(const std::wstring& sentence) {
    std::wstring modified_sentence = sentence;
    std::wsmatch match;
    modified_sentence = traditional_to_simplified(modified_sentence);//char_convert 繁体转简体

    modified_sentence = fullwidth_to_halfwidth(modified_sentence);//constants 全角转半角

     // number related NSW verbalization
     
     //chronology 日期
     while (std::regex_search(modified_sentence, match, RE_DATE))
         modified_sentence = replace_date(match);
  
     while (std::regex_search(modified_sentence, match, RE_DATE2))
         modified_sentence = replace_date2(match);

     // range first 时间

     while (std::regex_search(modified_sentence, match, RE_TIME_RANGE))
          modified_sentence = replace_time(match);

     while (std::regex_search(modified_sentence, match, RE_TIME))
         modified_sentence = replace_time(match);


     // 处理~波浪号作为至的替换
     //至
     while (std::regex_search(modified_sentence, match, re_to_range))
          modified_sentence = replace_to_range(match);
     //温度
     while (std::regex_search(modified_sentence, match, re_temperature))
          modified_sentence = replace_temperature(match);

     modified_sentence = replace_measure(modified_sentence);//quantifier

     //分数
     while (std::regex_search(modified_sentence, match, re_frac))
          modified_sentence = replace_frac(match);

     //百分比
     while (std::regex_search(modified_sentence, match, re_percentage))
          modified_sentence = replace_percentage(match);

     //手机
     while (std::regex_search(modified_sentence, match, re_mobile_phone) && is_valid_phone_number(modified_sentence, match))
         modified_sentence = process_mobile_number(modified_sentence);

     //固话
     while (std::regex_search(modified_sentence, match, re_telephone) && is_valid_phone_number(modified_sentence, match))
         modified_sentence = process_landline_number(modified_sentence);

     //400电话
     while (std::regex_search(modified_sentence, match, re_national_uniform_number)&& is_valid_phone_number(modified_sentence, match))
         modified_sentence = process_uniform_number(modified_sentence);

     //范围
     while (std::regex_search(modified_sentence, match, re_range))
         modified_sentence = replace_range(match);

    // 处理加减乘除
   
     while (std::regex_search(modified_sentence, match, re_asmd))
          modified_sentence = replace_asmd(match);

     // 负数
     //while (std::regex_search(modified_sentence, match, re_negative_num))
     //     modified_sentence = replace_negative_num(match);

     //纯数字
     //while (std::regex_search(modified_sentence, match, re_number))
     //     modified_sentence = replace_number(match);

     //正整数 + 量词
     //while (std::regex_search(modified_sentence, match, re_positive_quantifier))
     //     modified_sentence = replace_positive_quantifier(match);

     //编号-无符号整形
     //while (std::regex_search(modified_sentence, match, re_default_num))
     //     modified_sentence = replace_default_num(match);

     //通用的数字匹配
     while (std::regex_search(modified_sentence, match, re_number))
          modified_sentence = replace_number(match);

     // 调用 `post_replace` 函数
     modified_sentence = post_replace(modified_sentence);

     return modified_sentence;
}


std::vector<std::wstring> TextNormalizer::normalize(const std::wstring& text) {
    std::vector<std::wstring> sentences = split(text);
    for (auto& sentence : sentences) {
        sentence = normalize_sentence(sentence);
    }

    return sentences;
}

//int main() {
//#ifdef _WIN32
//    SetConsoleOutputCP(CP_UTF8);
//    std::wcout.imbue(std::locale("")); // 设置宽字符输出
//#endif
//
//    TextNormalizer normalizer;
//    std::wstring text = L"第三季度，Uber营收和每股收益均超出了分析师预期。华尔街分析师平均预计，Uber第三季度营收将达到109.8亿美元，每股收益将达到41美分。财报显示，Uber第三季度实际营收为111.9亿美元，每股收益1.20美元。";
//    std::vector<std::wstring> normalized_sentences = normalizer.normalize(text);
//
//    for (const auto& sentence : normalized_sentences) {
//        std::wcout << sentence << std::endl;
//    }
//
//    return 0;
//}