#include <iostream>
#include <locale>
#include <filesystem>
#include <utility>
#include <memory>
#include <gtest/gtest.h>
#ifdef _WIN32
#include <codecvt>
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#endif
#include <openvino/openvino.hpp>
//#include <openvino/op/transpose.hpp>
//#include <openvino/core/node.hpp>
#include "utils.h"
#define OV_MODEL_PATH "ov_models"

class TokenizerTestSuit : public ::testing::Test {
public:
    TokenizerTestSuit() {
        std::filesystem::path model_path = "C:\\Users\\gta\\source\\repos\\MeloTTS.cpp\\ov_models";
        std::filesystem::path bert_subword_tokenizer = model_path / ".." / "experimental" / "bert-base-multilingual-uncased" / "bert_subword" / "bert_subword_tokenizer.xml";
        std::filesystem::path bert_subword_detokenizer = model_path / ".." / "experimental" / "bert-base-multilingual-uncased" / "bert_subword" / "bert_subword_detokenizer.xml";
        std::filesystem::path dll_path = "C:\\Users\\gta\\source\\repos\\openvino_tokenizers_windows_2024.5.0.0_x86_64\\runtime\\bin\\intel64\\Release\\openvino_tokenizers.dll";
        core.add_extension(dll_path.string().c_str());
      
        subword_tokenizer_infer = core.compile_model(bert_subword_tokenizer.string()).create_infer_request();
        subword_detokenizer_infer = core.compile_model(bert_subword_detokenizer.string()).create_infer_request();
    }
    ov::Tensor tokenize(std::string&& prompt) {
        subword_tokenizer_infer.set_input_tensor(ov::Tensor{ ov::element::string, {1}, &prompt });
        subword_tokenizer_infer.infer();
        return  subword_tokenizer_infer.get_tensor("input_ids");
    }

    std::string* detokenize(int64_t&& token_id) {
        constexpr size_t BATCH_SIZE = 1;
        subword_detokenizer_infer.set_input_tensor(ov::Tensor{ ov::element::i64, {BATCH_SIZE,1}, &token_id });
        subword_detokenizer_infer.infer();
        return subword_detokenizer_infer.get_output_tensor().data<std::string>();
    }
    std::vector<std::string> detokenize(std::vector<int64_t>&& token_id, size_t size) {
        constexpr size_t BATCH_SIZE = 1;
        subword_detokenizer_infer.set_input_tensor(ov::Tensor{ ov::element::i64, {BATCH_SIZE,size}, token_id.data() });
        subword_detokenizer_infer.infer();
        size_t len = subword_detokenizer_infer.get_output_tensor().get_shape()[0];
        std::vector<std::string> res;
        std::string* ptr = subword_detokenizer_infer.get_output_tensor().data<std::string>();
        for (int i = 0; i < len; ++i)
            res.emplace_back(*(ptr + i));//subword_detokenizer_infer.get_output_tensor().data<std::string>()[i]
        return res;
    }
    std::vector<std::string> detokenize(ov::Tensor& token_ids) {
        subword_detokenizer_infer.set_input_tensor(token_ids);
        subword_detokenizer_infer.infer();
        size_t len = subword_detokenizer_infer.get_output_tensor().get_shape()[0];
        std::vector<std::string> res;
        std::string* ptr = subword_detokenizer_infer.get_output_tensor().data<std::string>();
        for (int i = 0; i < len; ++i) {
            // ignore the [CLS] token at the beginning and the [SEP] token at the end.
            if (*(ptr + i) == "[CLS]" || *(ptr + i) == "[SEP]")
                continue;
            res.emplace_back(*(ptr + i));
        }
        return res;
    }

    /**
     * @brief Tokenizes the given prompt into subwords.
     *
     * This function takes a string prompt and tokenizes it into subwords using the OpenVINO tokenizer.
     *
     * @param prompt The input string to be tokenized.
     * @return A vector of strings, where each string is a subword token.
     */
    std::vector<std::string> subword_tokenize(std::string& prompt) {
        ov::Tensor token_ids = tokenize(std::move(prompt));
        return detokenize(token_ids);
    }
    template<typename T>
    std::vector<T> get_output_vec(const ov::Tensor& output_tensor) {
        const T* output_data = output_tensor.data<T>();
        size_t frame_num = output_tensor.get_shape()[1];
        std::cout << output_tensor.get_shape() << std::endl;
        std::vector<T> res(frame_num);
        for (size_t i = 0; i < frame_num; ++i) {
            res[i] = output_data[i];
        }
        return res;
    }
protected:
    ov::Core core;
    ov::InferRequest subword_tokenizer_infer, subword_detokenizer_infer;


};


TEST_F(TokenizerTestSuit, BertSubwordTokenizer) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::string text = "编译器compiler会尽可能从函数实参function arguments推导缺失的模板实参template arguments";
    std::cout << text << std::endl;
    std::vector<std::string> tokens;
    std::vector<int64_t> token_ids;
    auto startTime = Time::now();
    ov::Tensor res = tokenize(std::move(text));
    auto vec = get_output_vec<int64_t>(res);
    auto execTime = get_duration_us_till_now(startTime);
    std::cout << "[INFO] subword_tokenize takes "<< execTime<<"us\n";
    const std::vector<int64_t> correct_ids = { 101, 6784, 7984, 2693, 85065, 33719, 1817, 3295, 2415, 6990, 1776, 2160, 4270, 3203, 2383, 18958, 59242, 4108, 3259, 6805,
        2981, 5975, 4767, 4508, 3203, 2383, 79947, 20849, 59242, 102 };

    EXPECT_EQ(vec, correct_ids);
}

TEST_F(TokenizerTestSuit, BertSubwordDeTokenizer) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    //system("chcp 65001"); //Using UTF-8 Encoding
    std::string text = "编译器compiler会尽可能从函数实参function arguments推导缺失的模板实参template arguments";

    std::vector<int64_t> input_ids = { 6784, 7984, 2693, 85065, 33719, 1817, 3295, 2415, 6990, 1776, 2160, 4270, 3203, 2383, 18958, 59242, 4108, 3259, 6805,
        2981, 5975, 4767, 4508, 3203, 2383, 79947, 20849, 59242 };
    std::vector<std::string> correct_subwords = {
    "编", "译", "器", "comp", "##iler", "会", "尽", "可", "能",
    "从", "函", "数", "实", "参", "function", "arguments",
    "推", "导", "缺", "失", "的", "模", "板", "实", "参",
    "temp", "##late", "arguments" };
    std::vector<std::string> res, res1;

    for (auto& id : input_ids) {
        res.emplace_back(*detokenize(std::move(id)));
    }

    res1 = detokenize(std::move(input_ids), input_ids.size());

    
    EXPECT_EQ(res, correct_subwords);
    EXPECT_EQ(res1, correct_subwords);
}
//https://github.com/huggingface/transformers/blob/main/docs/source/en/tokenizer_summary.md#subword-tokenization
TEST_F(TokenizerTestSuit, Subword_tokenization) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::string text = "编译器compiler会尽可能从函数实参function arguments推导缺失的模板实参template arguments";

    std::vector<std::string> correct_subwords = {
    "编", "译", "器", "comp", "##iler", "会", "尽", "可", "能",
    "从", "函", "数", "实", "参", "function", "arguments",
    "推", "导", "缺", "失", "的", "模", "板", "实", "参",
    "temp", "##late", "arguments" };
    auto startTime = Time::now();
    std::vector<std::string> res = subword_tokenize(text);
    auto execTime = get_duration_us_till_now(startTime);
    std::cout << "[INFO] split subword takes " << execTime << "us\n";

    EXPECT_EQ(res, correct_subwords);
}