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
#include <openvino/openvino.hpp>
#define OV_MODEL_PATH "ov_models"

class TokenizerTestSuit : public ::testing::Test {
public:
    TokenizerTestSuit() {
        std::filesystem::path model_path = "C:\\Users\\gta\\source\\repos\\MeloTTS.cpp\\ov_models";
        tokenizer_path = model_path / ".." / "experimental" / "bert-base-multilingual-uncased" / "openvino_tokenizer.xml";
        std::filesystem::path dll_path = "C:\\Users\\gta\\source\\repos\\openvino_tokenizers_windows_2024.5.0.0_x86_64\\runtime\\bin\\intel64\\Release\\openvino_tokenizers.dll";
        core.add_extension(dll_path.string().c_str());
        _infer_request = core.compile_model(tokenizer_path.string()).create_infer_request();
        
    }
    ov::Tensor tokenize(std::string&& prompt) {
        _infer_request.set_input_tensor(ov::Tensor{ ov::element::string, {1}, &prompt});
        _infer_request.infer();
        return  _infer_request.get_tensor("input_ids");
        return ov::Tensor{};
    }
    std::vector<int64_t> get_output_vec(const ov::Tensor& output_tensor) {

        const int64_t* output_data = output_tensor.data<int64_t>();
        size_t frame_num = output_tensor.get_shape()[1];
        std::cout << output_tensor.get_shape() << std::endl;
        std::vector<int64_t> res(frame_num, 0);
        for (int i = 0; i < frame_num; ++i) {
            res[i] = output_data[i];
        }
        return res;
    }
protected:
    ov::Core core;
    std::filesystem::path tokenizer_path;
    ov::InferRequest _infer_request;
    

};
TEST_F(TokenizerTestSuit, BasicTokenizer) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::string text = "编译器compiler会尽可能从函数实参function arguments推导缺失的模板实参template arguments";
    std::cout << text << std::endl;
    std::vector<std::string> tokens;
    std::vector<int64_t> token_ids;
    ov::Tensor res = tokenize(std::move(text));
    auto vec = get_output_vec(res);
    std::vector<int64_t> correct_ids = { 101, 6784, 7984, 2693, 85065, 33719, 1817, 3295, 2415, 6990, 1776, 2160, 4270, 3203, 2383, 18958, 59242, 4108, 3259, 6805,
        2981, 5975, 4767, 4508, 3203, 2383, 79947, 20849, 59242, 102 };

    EXPECT_EQ(vec,correct_ids);
}