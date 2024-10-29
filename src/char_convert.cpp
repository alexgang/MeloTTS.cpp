#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

// 定义简体和繁体字符
// 从文件中读取字符串
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}



    // 从文件中读取长字符串
    const std::string simplified_characters = readFile("simplified_characters.txt");
    const std::string traditional_characters = readFile("traditional_characters.txt");
    // 确保简体和繁体字符的长度相同
    //static_assert(simplified_characters.length() == traditional_characters.length(), "Character sets must be of the same length");


// 创建映射
std::unordered_map<char, char> s2t_dict;
std::unordered_map<char, char> t2s_dict;

// 初始化映射
void initialize_maps() {
    for (size_t i = 0; i < simplified_characters.length(); ++i) {
        s2t_dict[simplified_characters[i]] = traditional_characters[i];
        t2s_dict[traditional_characters[i]] = simplified_characters[i];
    }
}

// 将繁体转换为简体
std::string tranditional_to_simplified(const std::string& text) {
    std::string result;
    for (char ch : text) {
        if (t2s_dict.find(ch) != t2s_dict.end()) {
            result += t2s_dict[ch];
        }
        else {
            result += ch;
        }
    }
    return result;
}

// 将简体转换为繁体
std::string simplified_to_traditional(const std::string& text) {
    std::string result;
    for (char ch : text) {
        if (s2t_dict.find(ch) != s2t_dict.end()) {
            result += s2t_dict[ch];
        }
        else {
            result += ch;
        }
    }
    return result;
}

int main() {
    // 初始化映射
    initialize_maps();

    std::string text = "一般是指存取一個應用程式啟動時始終顯示在網站或網頁瀏覽器中的一個或多個初始網頁等畫面存在的站點";
    std::cout << "Original text: " << text << std::endl;

    std::string text_simple = tranditional_to_simplified(text);
    std::cout << "Simplified text: " << text_simple << std::endl;

    std::string text_traditional = simplified_to_traditional(text_simple);
    std::cout << "Traditional text: " << text_traditional << std::endl;

    return 0;
}