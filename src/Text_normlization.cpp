#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>

// 定义一个 TextNormalizer 类
class TextNormalizer {
public:
    TextNormalizer();
    std::vector<std::string> split(const std::string& text, const std::string& lang = "zh");
    std::string post_replace(const std::string& sentence);
    std::string normalize_sentence(const std::string& sentence);
    std::vector<std::string> normalize(const std::string& text);

private:
    std::regex SENTENCE_SPLITOR;
};

// 构造函数
TextNormalizer::TextNormalizer() : SENTENCE_SPLITOR(R"([：、，；。？！,;?!][”’]?)") {}

// 分割函数
std::vector<std::string> TextNormalizer::split(const std::string& text, const std::string& lang) {
    std::string modified_text = text;
    if (lang == "zh") {
        modified_text.erase(std::remove(modified_text.begin(), modified_text.end(), ' '), modified_text.end());
        modified_text = std::regex_replace(modified_text, std::regex(R"([——《》【】<>{}()（）#&@“”^_|\\])"), "");
    }
    modified_text = std::regex_replace(modified_text, SENTENCE_SPLITOR, "$1\n");
    modified_text.erase(std::remove(modified_text.begin(), modified_text.end(), '\n'), modified_text.end());

    std::vector<std::string> sentences;
    std::regex re(R"(\n+)");
    std::sregex_token_iterator it(modified_text.begin(), modified_text.end(), re, -1);
    std::sregex_token_iterator end;
    while (it != end) {
        sentences.push_back(it->str());
        ++it;
    }
    return sentences;
}

// 后处理替换函数
std::string TextNormalizer::post_replace(const std::string& sentence) {
    std::string modified_sentence = sentence;
    modified_sentence = std::regex_replace(modified_sentence, std::regex("/"), "每");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("①"), "一");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("②"), "二");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("③"), "三");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("④"), "四");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("⑤"), "五");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("⑥"), "六");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("⑦"), "七");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("⑧"), "八");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("⑨"), "九");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("⑩"), "十");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("α"), "阿尔法");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("β"), "贝塔");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("γ|Γ"), "伽玛");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("δ|Δ"), "德尔塔");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("ε"), "艾普西龙");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("ζ"), "捷塔");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("η"), "依塔");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("θ|Θ"), "西塔");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("ι"), "艾欧塔");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("κ"), "喀帕");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("λ|Λ"), "拉姆达");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("μ"), "缪");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("ν"), "拗");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("ξ|Ξ"), "克西");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("ο"), "欧米克伦");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("π|Π"), "派");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("ρ"), "肉");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("ς|σ|Σ"), "西格玛");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("τ"), "套");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("υ"), "宇普西龙");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("φ|Φ"), "服艾");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("χ"), "器");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("ψ|Ψ"), "普赛");
    modified_sentence = std::regex_replace(modified_sentence, std::regex("ω|Ω"), "欧米伽");
    modified_sentence = std::regex_replace(modified_sentence, std::regex(R"([-——《》【】<=>{}()（）#&@“”^_|\\])"), "");
    return modified_sentence;
}


std::string TextNormalizer::normalize_sentence(const std::string& sentence) {
    std::string modified_sentence = sentence;

    // 这里假设 `tranditional_to_simplified` 和其他转换函数已经实现
    // modified_sentence = tranditional_to_simplified(modified_sentence);
    // modified_sentence = modified_sentence.translate(F2H_ASCII_LETTERS).translate(F2H_DIGITS).translate(F2H_SPACE);

    // 调用 `post_replace` 函数
    modified_sentence = post_replace(modified_sentence);

    return modified_sentence;
}

std::vector<std::string> TextNormalizer::normalize(const std::string& text) {
    std::vector<std::string> sentences = split(text);
    for (auto& sentence : sentences) {
        sentence = normalize_sentence(sentence);
    }
    return sentences;
}