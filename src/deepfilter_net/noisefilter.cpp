/**
 * Copyright      2024    Vincent Liu (vincent1.liu@intel.com)
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
#ifdef USE_DEEPFILTERNET
#include "noisefilter.h"
#include <string>
#include <vector>
#include <iostream>

namespace melo {
  NoiseFilter::NoiseFilter()
  {
    std::cout << " NoiseFilter " << std::endl;
  }

  NoiseFilter::~NoiseFilter()
  {}

  void NoiseFilter::init(const std::string aModel_path,
                         const std::string aModel_device) {
  	/* can be DEEPFILTERNET2 or DEEPFILTERNET3 */
    std::cout << " NoiseFilter::init. aModel_path = " << aModel_path << " nf devices = " << aModel_device << std::endl;
  	auto dfnet_version = ov_deepfilternet::ModelSelection::DEEPFILTERNET3;
  	mDeepfilter.Init(aModel_path, aModel_device, dfnet_version, "nf_ov_cache");
  }

  void NoiseFilter::proc(std::vector<float>& aMamples) {
    torch::Tensor input_wav_tensor = torch::from_blob(aMamples.data(), { 1, (int64_t)aMamples.size() });
    aMamples = mDeepfilter.filter(input_wav_tensor);
  }
}
#endif // USE_DEEPFILTERNET