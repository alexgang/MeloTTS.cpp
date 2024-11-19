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
#pragma once
#ifndef OPENVOICE_TTS_H
#define OPENVOICE_TTS_H
#include "openvino_model_base.h"
namespace melo {
    class OpenVoiceTTS : public AbstractOpenvinoModel {
    public:
        OpenVoiceTTS(std::unique_ptr<ov::Core>& core_ptr, const std::filesystem::path& model_path, const std::string& device, const ov::AnyMap& config, const std::string& language) :
            AbstractOpenvinoModel(core_ptr, model_path, device, config), _language(language) {}

        OpenVoiceTTS() = default;
        std::vector<float> tts_infer(std::vector<int64_t>& phones, std::vector<int64_t>& tones, std::vector<int64_t>& lang_ids, 
            const std::vector<std::vector<float>>& phone_level_feature, const float& speed = 1.0,
            const int& speaker_id = 1, bool disable_bert = false, const float& sdp_ratio = 0.2f, const float& noise_scale = 0.6f, const float& noise_scale_w = 0.8f);
        virtual void ov_infer() override;
        virtual std::vector<float> get_ouput();
        void write_wave(const std::string& filename, int32_t sampling_rate, const float* samples, int32_t n);
        inline std::string get_language() { return _language; }
        static constexpr size_t BATCH_SIZE = 1;
    private:
        std::string _language = "ZH";
        
        int64_t _speakers = 1;//default speak id for zh
        float _noise_scale = 0.6f;
        float _length_scale = 1.00f;
        float _noise_scale_w = 0.80f;
        float _sdp_ration = 0.2f;
        
    };
}
#endif //OVOPENVOICETTS_H