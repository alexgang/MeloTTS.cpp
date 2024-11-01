#pragma once
#ifndef TEXT_NORMLIZATION_H
#define TEXT_NORMLIZATION_H
#include <vector>
#include <string>
#include <algorithm>

#include"char_convert.h"
#include"constant.h"
#include"chronology.h"
#include"number.h"
#include"phonecode.h"
#include"quantifier.h"

namespace text_normalization {
    class TextNormalizer {
    public:
        TextNormalizer();
        std::vector<std::wstring> split(const std::wstring& text, const std::wstring& lang = L"zh");
        std::wstring post_replace(const std::wstring& sentence);
        std::wstring normalize_sentence(const std::wstring& sentence);
        std::vector<std::wstring> normalize(const std::wstring& text);

    private:
        std::wregex SENTENCE_SPLITOR;
    };
}

#endif
