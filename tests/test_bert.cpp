#include <filesystem>
#include "bert.h"
#define MELO_TEST
#define OV_MODEL_PATH "ov_models"
int main() {
    std::filesystem::path model_dir = OV_MODEL_PATH;
    std::filesystem::path zh_bert_path = model_dir / "bert_ZH_static_int8.xml";
    std::cout << std::filesystem::absolute(zh_bert_path) << std::endl;
    std::cout << zh_bert_path.string() << std::endl;

    std::shared_ptr<melo::Tokenizer> tokenizer_ptr = std::make_shared<melo::Tokenizer>((model_dir / "vocab_bert.txt"));

    std::unique_ptr<ov::Core> core_ptr = std::make_unique<ov::Core>();
    melo::Bert zh_bert(core_ptr, zh_bert_path.string(), "CPU", "ZH", tokenizer_ptr);
    zh_bert.set_static_shape();

    //std::string text = "编译器compiler会尽可能从函数实参function arguments推导缺失的模板实参template arguments";
    //std::vector<int> word2ph{ 3, 4, 4, 4, 8, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 14, 20, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8, 6, 20, 2 };
    std::string text = "今天的meeting真的是超级productive";
/*
* ===torch_bert.var===
Mean per row: tensor([-0.0217, -0.0180, -0.0163, -0.0172, -0.0341, -0.0222, -0.0174, -0.0205,
        -0.0158, -0.0197, -0.0197, -0.0210,  0.0017])
Variance per row: tensor([0.8274, 0.7458, 0.6663, 0.6998, 1.3672, 0.8105, 0.7077, 0.7894, 0.6316,
        0.7086, 0.7134, 0.8239, 0.2795])

*/
    std::vector<int> word2ph{ 3, 4, 4, 4, 10, 4, 4, 4, 4, 4, 10, 8, 2 };
    std::vector<std::vector<float>> berts;
    zh_bert.get_bert_feature(text, word2ph, berts);
    std::cout << berts.size() << " "<< berts.front().size() << std::endl;

    return 0;
}