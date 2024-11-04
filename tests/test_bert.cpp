#include <iostream>
#include <filesystem>
#include <gtest/gtest.h>
#include "bert.h"
#define MELO_TEST
#define OV_MODEL_PATH "ov_models"
class BertTestSuit : public ::testing::Test {
public:
    BertTestSuit() {
        model_dir = OV_MODEL_PATH;
        zh_bert_path = model_dir / "bert_ZH_static_int8.xml";
        std::cout << "zh_bert_path:" << std::filesystem::absolute(zh_bert_path) << std::endl;
        std::unique_ptr<ov::Core> core_ptr = std::make_unique<ov::Core>();
        tokenizer_ptr = std::make_shared<melo::Tokenizer>((model_dir / "vocab_bert.txt"));
        zh_bert = melo::Bert(core_ptr, zh_bert_path.string(), "CPU", "ZH", tokenizer_ptr);
        zh_bert.set_static_shape();
    }
protected:
    std::filesystem::path model_dir;
    std::filesystem::path zh_bert_path;
    std::unique_ptr<ov::Core> core_ptr;
    melo::Bert zh_bert;
    std::shared_ptr<melo::Tokenizer> tokenizer_ptr;
   
};

TEST_F(BertTestSuit, StaticShapeModel){
#if  WIN32
    system("chcp 65001"); //Using UTF-8 Encoding
#endif //  WIN32    
    
    std::string text = "今天的meeting真的是超级productive";
    std::vector<int> word2ph{ 3, 4, 4, 4, 10, 4, 4, 4, 4, 4, 10, 8, 2 };
    std::vector<std::vector<float>> berts;
    zh_bert.get_bert_feature(text, word2ph, berts);
    //std::cout << berts.size() <<' '<< berts.front().size() << std::endl;
    EXPECT_EQ(berts.size(), 65);
    EXPECT_EQ(berts.front().size(),768);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv); // INT Google Test
    return RUN_ALL_TESTS();
}