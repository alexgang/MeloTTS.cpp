/**
 * Copyright      2024    Tong Qiu (tong.qiu@intel.com)
 *
 * See LICENSE for clarification regarding multiple authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include "cmudict.h"
namespace melo {

    // Constructor that loads data from a file.
    // @param filename The name of the file from which to load the data.
    CMUDict::CMUDict(const std::filesystem::path& filepath) {
        if (std::filesystem::exists(filepath)) {
            build_dict_from_txt(filepath.string());
            std::cout << "[INFO] CMUDict::CMUDict constructor build_dict_from_txt succeed\n";
        }
        else
            std::cerr << "CMUDict: file doesnot exits:" << filepath << std::endl;
    }
    void CMUDict::build_dict_from_txt(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "CMUDict::CMUDict: Cannot open file " << filename << std::endl;
            return;
        }
        // make da
        std::vector<const char*> _keys;
        std::vector<std::string> all_keys;
        std::vector<size_t> _length;
        std::vector<Darts::DoubleArray::result_type> _values;// dummy punctuation flag is 0

        std::string line;
        std::vector<std::vector<std::string>> inner_vec;  // store result of each line
        int i = 0;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key;
            if (!std::getline(iss, key, ':')) {
                continue; // Skip lines that cannot be parsed.
            }

            std::vector<std::vector<std::string>> value;
            std::string segment;

            while (std::getline(iss, segment, ',')) {
                std::istringstream segment_ss(segment);
                std::vector<std::string> subValues;
                std::string subValue;
                while (segment_ss >> subValue) {
                    subValues.push_back(subValue);
                }
                if (!subValues.empty()) {
                    value.push_back(subValues);
                }
            }

            if (!key.empty()) {
                // filter no engish lower case character (already in python script)
               // if (!std::all_of(key.begin(), key.end(), [&](auto& x) { return  'a' <= x && x <= 'z' || x =='\''; })) continue;
                all_keys.emplace_back(key);
                _length.emplace_back(key.length());
                _values.emplace_back(i++);
                values_data.emplace_back(value);
            }
        }

        file.close();

        for (auto& x : all_keys) _keys.emplace_back(x.c_str());

        // build da
        keys_da.build(_keys.size(), _keys.data(), _length.data(), _values.data());
        //keys_da.save("cmudict.dic");

        //test_da();
        std::cout << "CMDDict:: build double array trie.\n";
    }
    void CMUDict::test_da() {
        Darts::DoubleArray::result_pair_type  result_pair[16];
        size_t num = keys_da.commonPrefixSearch("microsoft", result_pair, sizeof(result_pair));
        std::cout << "found:" << num << std::endl;
        for (size_t i = 0; i < num; ++i) {
            std::cout << "\tvalue:" << result_pair[i].value << " matched key length:" << result_pair[i].length << std::endl;
            for (auto& row : values_data[result_pair[i].value]) for (auto& x : row) std::cout << x << ' ';
            std::cout << std::endl;
        }
        std::cout << "test da succeed!\n";
    }


    std::vector<std::vector<std::string>> CMUDict::wordBreak(const std::string& s) {
        std::unordered_map<int, std::vector<std::vector<std::string>>> ans;
        std::function<void(const std::string& s, int index)> backtrack = [&](const std::string& s, int index) {
            if (!ans.contains(index)) {
                if (index == s.size()) {
                    ans[index].push_back({});
                    return;
                }
                ans[index] = {};
                // assume string.size() >=2, because single-character words were excluded to reduce the number of permutations.
                for (int i = index + 2; i <= s.size(); ++i) {
                    const std::string word = s.substr(index, i - index);
                    if (contains(word)) {
                        backtrack(s, i);
                        for (const std::vector<std::string>& succ : ans[i]) {
                            ans[index].push_back({ word });
                            if(!succ.empty())
                                ans[index].back().insert(ans[index].back().end(), succ.begin(), succ.end());
                        }
                    }
                }
            }
            };

        backtrack(s, 0);
        return ans[0];
    }

   
}