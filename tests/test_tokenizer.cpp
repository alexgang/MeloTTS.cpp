#include <iostream>
#include <locale>
#include <filesystem>
#include <gtest/gtest.h>
#ifdef _WIN32
#include <codecvt>
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#endif

#include "tokenizer.h"
#define OV_MODEL_PATH "ov_models"
class TokenizerTestSuit : public ::testing::Test {
public:
    TokenizerTestSuit() {
        std::filesystem::path vocab_dir = std::filesystem::path(OV_MODEL_PATH) / "vocab_bert.txt";
        tokenizer = melo::Tokenizer(vocab_dir);
    }
protected:
    std::string vocab_dir;
    melo::Tokenizer tokenizer;

};
TEST_F(TokenizerTestSuit, BasicTokenizer) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::string text = "编译器compiler会尽可能从函数实参function arguments推导缺失的模板实参template arguments";
    std::cout << text << std::endl;
    std::vector<std::string> tokens;
    std::vector<int64_t> token_ids;
    tokenizer.Tokenize(text, tokens, token_ids);
#ifdef MELO_DEBUG
    for (const auto& word : tokens) std::cout << word << " ";
    std::cout << std::endl;
    for (const auto& id : token_ids) std::cout << id << " ";
    std::cout << std::endl;
#endif
    std::vector<std::string> correct_tokens = { "编", "译", "器", "comp", "##iler", "会", "尽", "可", "能", "从", "函", "数", "实", "参", "function", "arguments", "推", "导", "缺", "失", "的", "模", "板", "实", "参", "temp", "##late", "arguments", };
    std::vector<int64_t> correct_ids = { 101, 6784, 7984, 2693, 85065, 33719, 1817, 3295, 2415, 6990, 1776, 2160, 4270, 3203, 2383, 18958, 59242, 4108, 3259, 6805,
        2981, 5975, 4767, 4508, 3203, 2383, 79947, 20849, 59242, 102 };
    EXPECT_EQ(correct_tokens,tokens);
    EXPECT_EQ(correct_ids,token_ids);
}

TEST_F(TokenizerTestSuit,  WithPunctuation_0) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::string text = "咋样?";
    std::cout << text << std::endl;
    std::vector<std::string> tokens;
    std::vector<int64_t> token_ids;
    tokenizer.Tokenize(text, tokens, token_ids);
    std::vector<std::string> correct_tokens = { "咋", "样", "?", };
    std::vector<int64_t> correct_ids = { 101, 2502, 4586,  136,  102 };
    EXPECT_EQ(correct_tokens, tokens);
    EXPECT_EQ(correct_ids, token_ids);
}

TEST_F(TokenizerTestSuit, WithPunctuation_1) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::string text = "海内存知己,天涯若比邻.";
    std::cout << text << std::endl;
    std::vector<std::string> tokens;
    std::vector<int64_t> token_ids;
    tokenizer.Tokenize(text, tokens, token_ids);
    std::vector<std::string> correct_tokens = { "海", "内", "存", "知", "己", ",", "天", "涯", "若", "比", "邻", "."};
    std::vector<int64_t> correct_ids = { 101, 5097, 2093, 3161, 6091, 3441,  117, 2975, 5124, 7198, 4901, 8567, 119, 102 };
    EXPECT_EQ(correct_tokens, tokens);
    EXPECT_EQ(correct_ids, token_ids);
}